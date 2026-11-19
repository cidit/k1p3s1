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
    void *context;
    TaskState state;
    bool (*setup)(void *);
    bool (*loop)(void *);
    bool (*cleanup)(void *);
};

template <typename Context,
          bool (*Setup)(Context &),
          bool (*Loop)(Context &),
          bool (*Cleanup)(Context &)>
Task make_task(Context *ctx)
{
    return Task{
        .context = static_cast<void *>(ctx),
        .state = DONE,
        .setup = [](void *p) -> bool
        {
            return Setup(*static_cast<Context *>(p));
        },
        .loop = [](void *p) -> bool
        {
            return Loop(*static_cast<Context *>(p));
        },
        .cleanup = [](void *p) -> bool
        {
            return Cleanup(*static_cast<Context *>(p));
        }};
}

void task_run(Task &task)
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

#define CONCAT(a, b) a##b
#define XCONCAT(a, b) CONCAT(a, b)

#define TASK_CONTEXT() struct XCONCAT(TASK_TYPE, _context)
#define TASK_ctx XCONCAT(TASK_TYPE, _context)
#define TASK_INIT(...) XCONCAT(TASK_TYPE, _context) XCONCAT(TASK_TYPE, _init_context)(__VA_ARGS__)
#define TASK_SETUP() bool XCONCAT(TASK_TYPE, _setup)(XCONCAT(TASK_TYPE, _context) & ctx)
#define TASK_LOOP() bool XCONCAT(TASK_TYPE, _loop)(XCONCAT(TASK_TYPE, _context) & ctx)
#define TASK_CLEANUP() bool XCONCAT(TASK_TYPE, _cleanup)(XCONCAT(TASK_TYPE, _context) & ctx)

#define MAKE_TASK(task_type, ...)  \
    auto task_type##_CONTEXT = task_type##_init_context(__VA_ARGS__); \
    auto task_type##_TASK = make_task<                                \
        task_type##_context,                                          \
        task_type##_setup,                                            \
        task_type##_loop,                                             \
        task_type##_cleanup>(&task_type##_CONTEXT);                   

#define MAKE_TASK_NAMED(task_name, task_type, ...)                    \
    auto task_name##_CONTEXT = task_type##_init_context(__VA_ARGS__); \
    auto task_name##_TASK = make_task<                                \
        task_type##_context,                                          \
        task_type##_setup,                                            \
        task_type##_loop,                                             \
        task_type##_cleanup>(&task_name##_CONTEXT);                   
