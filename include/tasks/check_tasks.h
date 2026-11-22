#pragma once

#include "utils/task.h"
#include "fonctions.h"

#define TASK_TYPE CHECK_COLORS_DONE

TASK_CONTEXT()
{
    // bool to complete tasks
    bool &blue;
    bool &pink;
    bool &yellow;
    bool &green;
    bool &orange;
};

TASK_INIT(bool &blue,
          bool &pink,
          bool &yellow,
          bool &green,
          bool &orange)
{
    return (TASK_ctx){
        .blue = blue,
        .pink = pink,
        .yellow = yellow,
        .green = green,
        .orange = orange};
}

TASK_SETUP()
{
    // nothing to do
    return true;
}

TASK_LOOP()
{
    return (ctx.blue and ctx.pink and ctx.yellow and ctx.green and ctx.orange);
}

TASK_CLEANUP()
{
    lacher();
    return true;
}

#undef TASK_TYPE