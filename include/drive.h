#pragma once

#include <LibRobus.h>

float ticks_to_mm(float ticks_per_turn, float wheel_diam_mm, float tick_count) {
    auto circumference_mm = wheel_diam_mm * PI;
    auto rotation_turns = tick_count/ticks_per_turn;
    return rotation_turns * circumference_mm;
}