#pragma once

#include <Arduino.h>

#include "utils/task.h"
#include "utils/timer.h"
#include "utils/ema.h"
#include "utils/ring_buffer.h"
#include "utils/misc.h"
#include "LibRobus.h"
#include <drive.h>
#include <hardware.h>

TASK_CONTEXT(TRACK_SPEED)
{
    TimerContext & polling_timer;
    RingBuffer<2> &encoder_positions_right;
    RingBuffer<2> &encoder_positions_left;
    EmaContext & speed_ema_right;
    EmaContext & speed_ema_left;
    OdometryContext & odometry;
};

TASK_INIT(TRACK_SPEED)(
    TimerContext &polling_timer,
    RingBuffer<2> &encoder_positions_right,
    RingBuffer<2> &encoder_positions_left,
    EmaContext &speed_ema_right,
    EmaContext &speed_ema_left,
    OdometryContext &odometry)
{
    return TRACK_SPEED_context{
        .polling_timer = polling_timer,
        .encoder_positions_right = encoder_positions_right,
        .encoder_positions_left = encoder_positions_left,
        .speed_ema_right = speed_ema_right,
        .speed_ema_left = speed_ema_left,
        .odometry = odometry};
}

TASK_SETUP(TRACK_SPEED)
{
    // no setup required
    return true;
}

TASK_LOOP(TRACK_SPEED)
{
    auto now = micros();
    if (!timer_ready(ctx.polling_timer, now))
    {
        return false;
    }

    auto periods = timer_update_fixed(ctx.polling_timer, now);
    for (size_t i = 0; i < periods; i++)
    {
        auto right = ENCODER_Read(RIGHT);
        auto left = ENCODER_Read(LEFT);

        ring_buffer_append(ctx.encoder_positions_right, right);
        ring_buffer_append(ctx.encoder_positions_left, left);

        if (ctx.encoder_positions_right.full && ctx.encoder_positions_left.full)
        {
            auto r_delta_mm = ticks_to_mm(
                WHEEL_TICK_PER_TURN,
                WHEEL_DIAMETER,
                ring_buffer_read_nth(ctx.encoder_positions_right, 1) - ring_buffer_read_nth(ctx.encoder_positions_right, 0));
            auto r_mps = (r_delta_mm / 1000) / s_from_us(ctx.polling_timer.period);
            auto l_delta_mm = ticks_to_mm(
                WHEEL_TICK_PER_TURN,
                WHEEL_DIAMETER,
                ring_buffer_read_nth(ctx.encoder_positions_left, 1) - ring_buffer_read_nth(ctx.encoder_positions_left, 0));
            auto l_mps = (l_delta_mm / 1000) / s_from_us(ctx.polling_timer.period);

            EMA_add(ctx.speed_ema_right, r_mps);
            EMA_add(ctx.speed_ema_left, l_mps);

            odometry_update(ctx.odometry,
                            r_delta_mm, l_delta_mm,
                            WHEEL_DISTANCE_OUTER - WHEEL_THICKNESS);
        }
    }

    return false;
}

TASK_CLEANUP(TRACK_SPEED)
{
    // no cleanup required
    return true;
}
