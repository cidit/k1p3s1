#pragma once

/*
 * Odometry using 2 wheels.
 * https://en.wikipedia.org/wiki/Odometry
 *
 * Main reference: https://roboticsfaq.com/blog/wheel-odometry-explained
 */

#include <Arduino.h>
#include "utils/math.h"

struct OdometryContext
{
    Vec2D position_mm;
    float orientation_rad;
};

OdometryContext make_odometry() {
    return OdometryContext {
        .position_mm = Vec2D {.x = 0, .y=0},
        .orientation_rad = 0,
    };
}

void odometry_update(OdometryContext &ctx,
                     float distance_right_mm,
                     float distance_left_mm,
                     float track_width_mm)
{
    auto dd_mm = (distance_right_mm + distance_left_mm) / 2;
    auto da_rad = (distance_right_mm - distance_left_mm) / track_width_mm;
    ctx.position_mm.x += dd_mm * cos(ctx.orientation_rad + da_rad / 2);
    ctx.position_mm.y += dd_mm * sin(ctx.orientation_rad + da_rad / 2);
    ctx.orientation_rad += da_rad;
}