#pragma once

/**
 * /!\ this implementation works, but is fucking dogshit.
 */

enum TaskState
{
    SETUP,
    LOOP,
    DONE
};

struct Task
{
    void* context;
    TaskState state;
    bool (*setup)(void*);
    bool (*loop)(void*);
};

void run_task(Task &task)
{
    switch (task.state)
    {
    case SETUP:
    {
        auto done = task.setup(task.context);
        if (done)
        {
            task.state = LOOP;
        }
        break;
    }
    case LOOP:
    {
        auto done = task.loop(task.context);
        if (done)
        {
            task.state = DONE;
        }
        break;
    }
    case DONE:
        break;
    }
}

template<typename Context,
         bool (*Setup)(Context*),
         bool (*Loop)(Context*)>
Task make_task(Context* ctx) {
    return Task {
        .context = static_cast<void*>(ctx),
        .state = SETUP,
        .setup = [](void* p) -> bool {
            return Setup(static_cast<Context*>(p));
        },
        .loop = [](void* p) -> bool {
            return Loop(static_cast<Context*>(p));
        }
    };
}