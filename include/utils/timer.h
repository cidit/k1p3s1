#pragma once
#include <Arduino.h>

#include "utils/misc.h"

// TODO: update everything to support microsecond precision

struct TimerContext
{
    microseconds period;
    microseconds last_update;
    microseconds last_period;
};

TimerContext make_timer(microseconds period, microseconds start_at)
{
    return TimerContext{.period = period, .last_update = start_at, .last_period = start_at};
}

uint64_t timer_delta(const TimerContext &ctx, microseconds now)
{
    return now - ctx.last_update;
}

bool timer_ready(const TimerContext &ctx, microseconds now)
{
    return now - int64_t(ctx.last_period + ctx.period) >= 0;
}

/**
 * Updates the timer by stepping in fixed steps (defined py the timer's period). Useful when precise
 * timing is important.
 * @note Since this function can invalidate `timer_ready`, it should be called at the
 *       end of the `loop` if `timer_ready` is relied upon within it.
 * @returns the amount of periods elapsed since the last update.
 */
size_t timer_update_fixed(TimerContext &ctx, microseconds now)
{
    if (!timer_ready(ctx, now))
    {
        return 0;
    }
    auto time_since_last_period = now - ctx.last_period;
    auto rollovers = time_since_last_period / ctx.period;
    ctx.last_period += ctx.period * rollovers;
    ctx.last_update = now;
    return rollovers;
}

/**
 * Resets the timer so that it begins at `now`.
 */
void timer_reset(TimerContext &ctx, microseconds now)
{
    ctx.last_update = now;
    ctx.last_period = now;
}
