#pragma once

#include <Arduino.h>

#include "utils/task.h"
#include "utils/timer.h"
#include "utils/ema.h"
#include "utils/misc.h"
#include "LibRobus.h"

struct CalibrationTaskContext
{
    TimerContext timer;
    float step_size;
    int bounces;
    int bounce_idx;
    int bounce_direction;
    float current_power;
    EmaContext *right_mps;
    EmaContext *left_mps;
};

CalibrationTaskContext calibration_task_ctx = CalibrationTaskContext {
    .timer = make_timer(500, 0),
    .step_size = 0.01,
    .bounces = 10,
    .bounce_idx = 0,
    .bounce_direction = 1,
    .current_power = 0,
    .right_mps = nullptr,
    .left_mps = nullptr,
};

bool setup_calibration_task(CalibrationTaskContext *ctx)
{
    Serial.println("Characterization started");
    return true;
}

bool loop_calibration_task(CalibrationTaskContext *ctx)
{
    auto now = millis();
    if (!timer_ready(ctx->timer, now))
    {
        return false;
    }
    timer_reset(ctx->timer, now);
    
    if (!ctx->right_mps || !ctx->left_mps || !ctx->right_mps->ready || !ctx->left_mps->ready) {
        return false;
    }
    GRAPHT(ctx->right_mps->current)
    GRAPHT(ctx->left_mps->current)
    GRAPHT(ctx->current_power)

    MOTOR_SetSpeed(RIGHT, ctx->current_power);
    MOTOR_SetSpeed(LEFT, ctx->current_power);

    ctx->current_power += ctx->step_size * ctx->bounce_direction;

    if (abs(ctx->current_power-1.0) < 0.001) {
        ctx->bounce_direction = -1;
        ctx->bounce_idx +=1;
    } else if (abs(ctx->current_power+1.0) < 0.001) {
        ctx->bounce_direction = 1;
        ctx->bounce_idx +=1;
    }
    
    return ctx->bounce_idx == ctx->bounces && abs(ctx->current_power) < 0.001;
    
}

bool cleanup_calibration_task(CalibrationTaskContext*ctx) {

    MOTOR_SetSpeed(RIGHT, 0);
    MOTOR_SetSpeed(LEFT, 0);
    Serial.println("finished characterization");
    return true;
}


Task calibration_task = make_task<
    CalibrationTaskContext,
    setup_calibration_task,
    loop_calibration_task,
    cleanup_calibration_task
>(&calibration_task_ctx);