#pragma once

#include "utils/task.h"

TASK_CONTEXT(IDLE) {

};

TASK_INIT(IDLE) (/* no arguments*/) {
    return (IDLE_context) {};
}

TASK_SETUP(IDLE) {
    // nothing to do
    return false;
}


TASK_LOOP(IDLE) {
    // nothing to do
    return false;
}


TASK_CLEANUP(IDLE) {
    // nothing to do
    return false;
}