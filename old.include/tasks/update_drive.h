#pragma once

#include "utils/task.h"
#include "utils/timer.h"
#include "drive.h"

#define TASK_TYPE UPDATE_DRIVE

TASK_CONTEXT()
{
    TimerContext & timer;
    Drive & drive;
};

TASK_INIT(TimerContext &timer, Drive &drive)
{
    return (TASK_ctx){
        .timer = timer,
        .drive = drive,
    };
}

TASK_SETUP()
{
    return true;
}

TASK_LOOP()
{
    auto now = micros();
    if (!timer_ready(ctx.timer, now))
    {
        return false;
    }

    auto dt_s = timer_delta(ctx.timer, now);
    timer_update_fixed(ctx.timer, now);

    motor_correct_power(ctx.drive.left, dt_s);
    motor_correct_power(ctx.drive.right, dt_s);
    return false;
}

TASK_CLEANUP()
{
    // nothing to do
    return true;
}

#undef TASK_TYPES