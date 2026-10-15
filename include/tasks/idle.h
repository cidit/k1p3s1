#pragma once

#include "utils/task.h"

struct IdleContext {} idle_task_ctx;

bool idle_no_op(IdleContext*ctx) { return false; }

Task IDLE_TASK = make_task<
    IdleContext,
    idle_no_op,
    idle_no_op,
    idle_no_op
>(&idle_task_ctx);