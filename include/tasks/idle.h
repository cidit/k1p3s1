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
    return false;
}


TASK_LOOP() {
    // nothing to do
    return false;
}


TASK_CLEANUP() {
    // nothing to do
    return false;
}

#undef TASK_TYPE