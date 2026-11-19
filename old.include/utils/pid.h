/*
 * Pid implementation.
 * References:
 * - Wikipedia
 * - https://arxiv.org/pdf/2604.15918
 */

#pragma once
#include <Arduino.h>
#include <utils/misc.h>

struct PidK
{
    float p, i, d;
};

float pidk_get_Ti(const PidK &k)
{
    if (k.i == 0)
        return NAN;
    return k.p / k.i;
}

void pidk_set_Ti(PidK &k, seconds Ti)
{
    k.i = Ti == 0 ? 0 : k.p / Ti;
}

float pidk_get_Td(const PidK &k)
{
    if (k.p == 0)
        return NAN;
    return k.d / k.p;
}

void pidk_set_Td(PidK &k, seconds Td)
{
    k.d = k.p * Td;
}

/**
 * bit flags
 */
enum PidTweaks : uint16_t
{
    D_ON_PV = 1,
    CAP_INTEGRAL = 2,
};

struct PidContext
{
    PidK k;
    float last_error;
    float last_input;
    float error_sum;
    float setpoint;
    float i_min, i_max;
    uint8_t tweaks;
    bool initialized;
    bool enabled;
};

PidContext make_pid(PidK k, uint8_t tweaks)
{
    return PidContext{
        .k = k,
        .last_error = 0,
        .last_input = 0,
        .error_sum = 0,
        .setpoint = 0,
        .i_min = 0,
        .i_max = 0,
        .tweaks = tweaks,
        .initialized = false,
        .enabled = false};
}

void pid_reset(PidContext &ctx)
{
    ctx.error_sum = 0;
    ctx.last_error = 0;
    ctx.last_input = 0;
    ctx.initialized = false;
}

void pid_disable(PidContext & ctx)
{
    pid_reset(ctx);
    ctx.enabled = false;
}

float pid_compute_next(PidContext &ctx, float input, float dt_s)
{
    if (!ctx.enabled) {
        // Error case, don't know how to handle.
        return 0;
    }

    auto error = ctx.setpoint - input;

    auto p_term = ctx.k.p * error;

    ctx.error_sum += error * dt_s;
    // We want the i_term to be able to saturate the actuator but never
    // more than that. Otherwise, it'll wind up.
    if (ctx.tweaks & PidTweaks::CAP_INTEGRAL &&
        ctx.k.i != 0)
    {
        auto max_err_sum = ctx.i_max / ctx.k.i;
        auto min_err_sum = ctx.i_min / ctx.k.i;
        if (ctx.error_sum > max_err_sum)
        {
            ctx.error_sum = max_err_sum;
        }
        else if (ctx.error_sum < min_err_sum)
        {
            ctx.error_sum = min_err_sum;
        }
    }

    auto i_term = ctx.k.i * ctx.error_sum;

    auto d_term = ctx.tweaks & PidTweaks::D_ON_PV
                      ? ctx.k.d * (ctx.last_input - input) / dt_s
                      : ctx.k.d * (error - ctx.last_error) / dt_s;

    // If the PID has just started and is on D_ON_PV mode, the d_term
    // should not apply because we are missing data to establish a trend.
    if (!ctx.initialized) {
        d_term = 0;
        ctx.initialized = true;
    }

    ctx.last_input = input;
    ctx.last_error = error;

    auto output = p_term + i_term + d_term;
    return output;
}