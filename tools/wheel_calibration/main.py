import pathlib
import textwrap
from argparse import ArgumentParser

import numpy as np
import plotly.express as px
import plotly.graph_objects as go
import polars as pl


def main():
    parser = ArgumentParser()
    parser.add_argument("-i", "--input_file")
    parser.add_argument("-o", "--output_file")
    parser.add_argument("--resampling_size", default=50)
    parser.add_argument("--show", action="store_true")
    args = parser.parse_args()

    resampling_size = int(args.resampling_size)

    # format: timestamp(ms),ctx->right_mps->current,ctx->left_mps->current,ctx->current_power,
    df = (
        pl.read_csv(args.input_file)
        .drop("")
        .rename(
            {
                "timestamp(ms)": "ms",
                "ctx->right_mps->current": "right",
                "ctx->left_mps->current": "left",
                "ctx->current_power": "power",
            }
        )
    ).remove(pl.col("power") == 0)

    left_trimmed = trim_unimpactful_and_unreliable(
        df.select("power", "left").rename({"left": "speed"}).sort("speed")
    )
    left_deadzone, left_resampled = resample_calibration(left_trimmed, resampling_size)

    right_trimmed = trim_unimpactful_and_unreliable(
        df.select("power", "right").rename({"right": "speed"})
    )
    right_deadzone, right_resampled = resample_calibration(right_trimmed, resampling_size)

    if args.output_file:
        file_path = pathlib.Path(args.output_file)
        if file_path.suffix == ".h":
            with open(file_path, "w") as f:
                f.writelines(
                    textwrap.dedent(f"""
                        #pragma once
                        #include <utils/calib.h>

                        float __left_wheel_x[] = {{{left_resampled["speed"].str.join(",")[0]}}};
                        float __left_wheel_y[] = {{{left_resampled["power"].str.join(",")[0]}}};
                        float __right_wheel_x[] = {{{right_resampled["speed"].str.join(",")[0]}}};
                        float __right_wheel_y[] = {{{right_resampled["power"].str.join(",")[0]}}};

                        auto left_wheel = CalibrationProfile {{
                            .size = {resampling_size},
                            .x = __left_wheel_x,
                            .y = __left_wheel_y,
                            .deadzone = {{{left_deadzone[0]}, {left_deadzone[1]}}},
                        }};
                        auto right_wheel = CalibrationProfile {{
                            .size = {resampling_size},
                            .x = __right_wheel_x,
                            .y = __right_wheel_y,
                            .deadzone = {{{right_deadzone[0]}, {right_deadzone[1]}}},
                        }};
                    """)
                )

    if args.show:
        fig = px.line(left_resampled, y="power", x="speed")
        fig.show()
        fig = px.line(right_resampled, y="power", x="speed")
        fig.show()


def trim_unimpactful_and_unreliable(df: pl.DataFrame) -> pl.DataFrame:
    # we remove powers that dont reliably produce a speed
    useful_powers = (
        df.with_columns(pl.col("speed").abs())
        .group_by("power")
        .agg(pl.min("speed"))
        .remove(pl.col("speed") == 0)
    )["power"]

    return df.filter(pl.col("power").is_in(useful_powers.implode()))


def resample_calibration(
    calibration: pl.DataFrame, num_samples: int
) -> tuple[tuple[float, float], pl.DataFrame]:
    negative = calibration.filter(pl.col("speed") < 0)
    negative_resampled = polynomial_resample(
        negative["speed"],
        negative["power"],
         num_samples // 2
    )

    positive = calibration.filter(pl.col("speed") > 0)
    positive_resampled = polynomial_resample(
        positive["speed"],
        positive["power"],
        num_samples // 2
    )

    deadzone = negative[-1, "speed"], positive[0, "speed"]
    resampled = negative_resampled.vstack(positive_resampled).rename(
        {"x": "speed", "y": "power"}
    )
    return deadzone, resampled


def polynomial_resample(x, y, n) -> pl.DataFrame:
    x, y = np.polynomial.Polynomial.fit(x, y, deg=2).linspace(n)
    return pl.DataFrame({"x": x, "y": y})


if __name__ == "__main__":
    main()
