#pragma once

#include <LibRobus.h>
#include <utils/pid.h>
#include <utils/calib.h>
#include <utils/ema.h>

float ticks_to_mm(float ticks_per_turn, float wheel_diam_mm, float tick_count) {
    auto circumference_mm = wheel_diam_mm * PI;
    auto rotation_turns = tick_count/ticks_per_turn;
    return rotation_turns * circumference_mm;
}

struct Motor {
    uint8_t id;
    PidContext speed_correction;
    EmaContext speed_calculation;
    CalibrationProfile power_profile;
};

float motor_get_symetric_min_speed(const Motor &motor) {
    auto min_negative = motor.power_profile.deadzone[0];
    auto min_positive = motor.power_profile.deadzone[0];
    return max(abs(min_negative), abs(min_positive));
}

float motor_get_symetric_max_speed(const Motor &motor) {
    auto max_negative = motor.power_profile.x[0];
    auto max_positive = motor.power_profile.x[motor.power_profile.size-1];
    return min(abs(max_negative), abs(max_positive));
}

/**
 * @returns whether or not the speed was set correctly.
 */
bool motor_set_speed(Motor &motor, float speed_mps, bool feed_forward) {
    bool speed_too_high = abs(speed_mps) > motor_get_symetric_max_speed(motor);
    bool intentionnal_stop = speed_mps == 0;
    bool speed_too_low = abs(speed_mps) < motor_get_symetric_min_speed(motor) && !intentionnal_stop;

    if (speed_too_high || speed_too_low) {
        return false;
    }
    
    // feed forward
    auto power = lin_interpolate(motor.power_profile, speed_mps);
    MOTOR_SetSpeed(motor.id, power);
    
    // updating the PID
    pid_reset(motor.speed_correction);
    motor.speed_correction.setpoint = speed_mps;

    return true;
}

struct Drive {
    Motor left, right;
};

