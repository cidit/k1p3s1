#pragma once
#include <Arduino.h>

#include "utils/misc.h"

struct TimerContext
{
    timestamp_t period_ms;
    timestamp_t last_update_ms;
    timestamp_t last_period_ms;
};

TimerContext make_timer(timestamp_t period_ms, timestamp_t start_at_ms)
{
    return TimerContext{.period_ms = period_ms, .last_update_ms = start_at_ms, .last_period_ms = start_at_ms};
}

uint64_t timer_delta(const TimerContext &ctx, timestamp_t now)
{
    return now - ctx.last_update_ms;
}

bool timer_ready(const TimerContext &ctx, timestamp_t now)
{
    return now - int64_t(ctx.last_period_ms + ctx.period_ms) >= 0;
}

/**
 * Updates the timer by stepping in fixed steps (defined py the timer's period). Useful when precise
 * timing is important.
 * @note Since this function can invalidate `timer_ready`, it should be called at the
 *       end of the `loop` if `timer_ready` is relied upon within it.
 * @returns the amount of periods elapsed since the last update.
 */
size_t timer_update_fixed(TimerContext &ctx, timestamp_t now)
{
    if (!timer_ready(ctx, now))
    {
        return 0;
    }
    auto time_since_last_period = now - ctx.last_period_ms;
    auto rollovers = time_since_last_period / ctx.period_ms;
    ctx.last_period_ms += ctx.period_ms * rollovers;
    ctx.last_update_ms = now;
    return rollovers;
}

/**
 * Resets the timer so that it begins at `now`.
 */
void timer_reset(TimerContext &ctx, timestamp_t now)
{
    ctx.last_update_ms = now;
    ctx.last_period_ms = now;
}
