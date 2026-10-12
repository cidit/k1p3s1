#pragma once
#include <Arduino.h>

#include "utils/misc.h"

struct TimerContext
{
    timestamp_t period_ms;
    timestamp_t last_mm;
};

TimerContext make_timer(timestamp_t period_ms, timestamp_t start_at_ms)
{
    return TimerContext{.period_ms = period_ms, .last_mm = start_at_ms};
}

/**
 * @returns (negative) The time left until the timer triggers again, in milliseconds.
 * 
 *          (positive) The time since the timer has last triggered, in milliseconds.
 */
int64_t timer_delta(const TimerContext &ctx, timestamp_t now)
{
    return now - int64_t(ctx.last_mm + ctx.period_ms);
}

bool timer_ready(const TimerContext &ctx, timestamp_t now)
{
    return timer_delta(ctx, now) >= 0;
}

/**
 * @returns (negative) The amount of periods to go before the specified `time`. (I.E. The amount 
 *                     of times the timer is gonna go off before the time is reached.)
 * 
 *          (positive) The amount of periods that have happened since the specified `time`.
 */
float timer_periods_since(const TimerContext &ctx, timestamp_t time)
{
    auto time_elapsed = ctx.last_mm - int64_t(time);
    return time_elapsed / float(ctx.period_ms);
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
    auto time_since_last = now - ctx.last_mm;
    auto rollovers = time_since_last / ctx.period_ms;
    ctx.last_mm += ctx.period_ms * rollovers;
    return rollovers;
}

/**
 * Resets the timer so that it begins at `now`.
 */
void timer_reset(TimerContext &ctx, timestamp_t now)
{
    ctx.last_mm = now;
}
