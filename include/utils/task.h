#pragma once

/**
 * /!\ this implementation works, but is fucking dogshit.
 */

enum TaskState
{
    SETUP,
    LOOP,
    CLEANUP,
    DONE
};

struct Task
{
    void* context;
    TaskState state;
    bool (*setup)(void*);
    bool (*loop)(void*);
    bool (*cleanup)(void*);
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
            task.state = CLEANUP;
        }
        break;
    }
    case CLEANUP:
    {
        auto done = task.cleanup(task.context);
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
         bool (*Loop)(Context*),
         bool (*Cleanup)(Context*)
         >
Task make_task(Context* ctx) {
    return Task {
        .context = static_cast<void*>(ctx),
        .state = SETUP,
        .setup = [](void* p) -> bool {
            return Setup(static_cast<Context*>(p));
        },
        .loop = [](void* p) -> bool {
            return Loop(static_cast<Context*>(p));
        },
        .cleanup = [](void* p) -> bool {
            return Cleanup(static_cast<Context*>(p));
        }
    };
}