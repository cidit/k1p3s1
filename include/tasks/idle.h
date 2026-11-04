#pragma once

#include "utils/task.h"

#define TASK_TYPE IDLE

TASK_CONTEXT() {

};

TASK_INIT() {
    return (IDLE_context) {};
}

TASK_SETUP() {
    // nothing to do
    return true;
}


TASK_LOOP() {
    // nothing to do
    return true;
}


TASK_CLEANUP() {
    // nothing to do
    return true;
}

#undef TASK_TYPE