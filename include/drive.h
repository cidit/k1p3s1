#pragma once

#include <LibRobus.h>
#include "util.h"

const float WHEEL_CIRCUMFERENCE = 0.2393893602; // m
const float WHEEL_DISTANCES = 0.18359; // m
const float TICKS_PER_TURNS = 3200.0;

struct Motor {
    // pins are actually the ids used by librobus.
    pin_t motor_pin, enco_pin;
    bool reversed;
    float proportionnal_scaling_factor;
};

void motor_write(const Motor& motor, float speed) {
    if (motor.reversed) {
        speed = -speed;
    }
    MOTOR_SetSpeed(motor.motor_pin, speed * motor.proportionnal_scaling_factor);
}

struct Drive {
    Motor left, right;
};

void forward(const Drive& drive, float meters) {
    auto ticks_to_travel = meters / WHEEL_CIRCUMFERENCE * TICKS_PER_TURNS;
    auto current_left = ENCODER_Read(LEFT);
    auto current_right = ENCODER_Read(RIGHT);

    motor_write(drive.left, 0.5);
}