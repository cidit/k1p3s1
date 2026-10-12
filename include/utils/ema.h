#pragma once

/*
 * Exponential Moving Average
 * https://en.wikipedia.org/wiki/Exponential_smoothing#Double_exponential_smoothing_(Holt_linear)
 */

#include <Arduino.h>
#include "utils/misc.h"


struct EmaContext
{
    bool ready;
    float current;
    float alpha;
};

EmaContext make_EMA(
    timestamp_t period_ms,
    float time_constant_s)
{
    auto period_s = float(period_ms) / 1000;
    auto alpha = 1 - exp(-period_s / time_constant_s);

    return (EmaContext){
        .ready = false,
        .current = 0,
        .alpha = alpha,
    };
}

void EMA_add(EmaContext&ctx, float new_point) {
    if (ctx.ready) {
        ctx.current = ctx.alpha*new_point + (1-ctx.alpha)*ctx.current;
    } else {
        ctx.current = new_point;
        ctx.ready = true; // we now have at least one value
    }
}