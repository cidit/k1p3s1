#pragma once
#include <Arduino.h>

struct PidK
{
    float p, i, d;
};

float pidk_get_Ti(const PidK &k)
{
    return k.p / k.i;
}

void pidk_set_Ti(PidK &k, float Ti)
{
    k.i = k.p / Ti;
}

float pidk_get_Td(const PidK &k)
{
    return k.i / k.p;
}

void pidk_set_Td(PidK &k, float Td)
{
    k.d = Td / k.i;
}

/**
 * bit flags
 */
enum PidTweaks: uint16_t
{
    D_ON_PV = 1, 
};

struct PidContext
{
    PidK k;
    float last_error;
    float error_sum;
    float setpoint;
    uint8_t tweaks;
};

PidContext make_pid(PidK k, uint8_t tweaks) {
    return PidContext {
        .k = k,
        .last_error = 0,
        .error_sum = 0,
        .setpoint = 0,
        .tweaks = tweaks
    };
}

void pid_clamp_integral(PidContext &ctx, float min, float max)
{
    if (ctx.error_sum > max)
    {
        ctx.error_sum = max;
    }
    else if (ctx.error_sum < min)
    {
        ctx.error_sum = min;
    }
}

void pid_reset(PidContext &ctx) {
    ctx.error_sum = 0;
    ctx.last_error = 0;
    ctx.setpoint = 0;
}

float pid_compute_next(PidContext &ctx, float input, float dt_s)
{
    auto error = ctx.setpoint - input;
    ctx.error_sum += error;
    auto p_term = ctx.k.p * error;
    auto i_term = ctx.k.i * ctx.error_sum * dt_s;
    auto d_term = ctx.k.p * (ctx.last_error - error) / dt_s;
    ctx.last_error = error;
    return p_term + i_term + d_term;
}