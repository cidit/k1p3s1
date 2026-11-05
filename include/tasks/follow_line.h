#pragma once


#include "utils/task.h"

#define TASK_TYPE FOLLOW_LINE

TASK_CONTEXT() {
    float &advance_at_mps;
};

TASK_INIT(float &advance_at_mps) {
    return (TASK_ctx) {.advance_at_mps = advance_at_mps};
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