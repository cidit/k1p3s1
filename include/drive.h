#pragma once

#include <LibRobus.h>
#include "utils/misc.h"

const float WHEEL_CIRCUMFERENCE = 0.2393893602; // m
const float WHEEL_DISTANCES = 0.18359; // m
const float TICKS_PER_TURNS = 3200.0;
const float TICKS_PER_METER = TICKS_PER_TURNS * WHEEL_CIRCUMFERENCE; 
const float DIAMETRIC_DISTANCE = 0.0; // m

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

void march_by() {

}

void rotate(
    const Drive& drive, 
    float rotation_clockwise, // rad
) {
    auto current_left = ENCODER_Read(LEFT);
    auto current_right = ENCODER_Read(RIGHT);
    
    auto circumference = DIAMETRIC_DISTANCE * PI;
    auto distance_per_wheel = (circumference/2*PI) * rotation_clockwise;
    auto ticks = 
}