#pragma once

#include <Arduino.h>
#include "utils/timer.h"
#include "utils/task.h"
#include "utils/ema.h"
#include "line_follower.h"

#define TASK_TYPE TRACK_LINE

static const int LEN = 3;
TASK_CONTEXT()
{
    TimerContext &timer;
    uint8_t *pins; // SHOULD BE 3 ELEMENTS LONG
    MappingParameters *parameters;
    EmaContext *emas;
};

TASK_INIT(TimerContext &timer, uint8_t *pins, MappingParameters *parameters, EmaContext *emas)
{
    return (TASK_ctx){
        .timer = timer,
        .pins = pins,
        .parameters = parameters,
        .emas = emas,
    };
}

TASK_SETUP()
{
    // nothing to do
    return true;
}

TASK_LOOP()
{
    auto now = micros();
    if (!timer_ready(ctx.timer, now))
    {
        return false;
    }
    Serial.println("doin");

    timer_reset(ctx.timer, now);

    int raw[LEN] = {0};

    for (size_t i = 0; i < LEN; i++)
    {
        raw[i] = analogRead(ctx.pins[i]);
    }

    int offset[LEN] = {0};
    pipe_offset(ctx.parameters, raw, offset, LEN);

    float scaled[LEN] = {0};
    pipe_scaled(ctx.parameters, offset, scaled, LEN);

    float clamped[LEN] = {0};
    pipe_clamped(scaled, clamped, LEN);

    for (size_t i = 0; i < LEN; i++)
    {
        EMA_add(ctx.emas[i], clamped[i]);
    }

    return false;
}

TASK_CLEANUP()
{
    // nothing to do
    return true;
}

#undef TASK_TYPE