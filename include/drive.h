#pragma once

#include <LibRobus.h>
#include <utils/pid.h>
#include <utils/calib.h>
#include <utils/ema.h>
#include <hardware.h>

float ticks_to_mm(float ticks_per_turn, float wheel_diam_mm, float tick_count)
{
    auto circumference_mm = wheel_diam_mm * PI;
    auto rotation_turns = tick_count / ticks_per_turn;
    return rotation_turns * circumference_mm;
}

struct Motor
{
    uint8_t id;
    bool reversed; // TODO: apply that factor.
    float applied_power;
    PidContext &speed_correction;
    EmaContext &speed_calculation;
    CalibrationProfile &power_profile;
};

Motor make_motor(
    uint8_t id,
    bool reversed,
    PidContext &speed_correction,
    EmaContext &speed_calculation,
    CalibrationProfile &power_profile)
{
    return Motor{
        .id = id,
        .reversed = reversed,
        .applied_power = 0,
        .speed_correction = speed_correction,
        .speed_calculation = speed_calculation,
        .power_profile = power_profile,
    };
}

float motor_get_symetric_min_speed(const Motor &motor)
{
    auto min_negative = motor.power_profile.deadzone[0];
    auto min_positive = motor.power_profile.deadzone[1];
    return max(abs(min_negative), abs(min_positive));
}

float motor_get_symetric_max_speed(const Motor &motor)
{
    auto max_negative = motor.power_profile.x[0];
    auto max_positive = motor.power_profile.x[motor.power_profile.size - 1];
    return min(abs(max_negative), abs(max_positive));
}

void motor_set_power(Motor &motor, float power)
{
    // motor.applied_power = power;
    MOTOR_SetSpeed(motor.id, motor.reversed ? -power : power);
}

/**
 * @returns whether or not the speed was set correctly.
 */
bool motor_set_speed(Motor &motor, float speed_mps)
{
    bool speed_too_high = abs(speed_mps) > motor_get_symetric_max_speed(motor);
    bool intentionnal_stop = speed_mps == 0;
    bool speed_too_low = abs(speed_mps) < motor_get_symetric_min_speed(motor) && !intentionnal_stop;

    if (speed_too_high || speed_too_low)
    {
        return false;
    }

    motor.speed_correction.enabled = true;

    if (intentionnal_stop)
    {
        motor_set_power(motor, 0);
        pid_reset(motor.speed_correction);
        motor.speed_correction.setpoint = 0; // BRAKE
        return true;
    }

    // FIXME: feed-forward at t=0 introduces a transiant state where the system doesnt have the time
    // to react fast enough for the PID to start going crazy. maybe introduce a setpoint ramp
    // with a relation to the EMA's time constant? (time from last state change -> closer to the
    // length of the time constant should represent the real setpoint more)
    // FOR NOW, we'll just hope the D term compensates enough.

    // feed forward
    auto power = lin_interpolate(motor.power_profile, speed_mps) * (motor.reversed? -1: 1);
    motor.applied_power = power;
    motor_set_power(motor, power);

    // updating the PID
    pid_reset(motor.speed_correction); // TODO: is this important, or should it be ommited?
    motor.speed_correction.setpoint = speed_mps;

    return true;
}

void motor_set_neutral(Motor &motor)
{
    motor_set_power(motor, 0);
    pid_disable(motor.speed_correction);
}

float motor_correct_power(Motor &motor, float dt_s)
{
    if (!motor.speed_correction.enabled)
    {
        return 0;
    }

    auto current_speed = motor.speed_calculation.current * (motor.reversed? -1: 1);
    auto power_correction = pid_compute_next(motor.speed_correction, current_speed, dt_s);
    motor_set_power(motor, motor.applied_power + power_correction);
    return power_correction;
}

struct Drive
{
    Motor &left;
    Motor &right;
    PidContext &rotation;
    float rotation_start_right, rotation_start_left, target_rotation;
};

Drive make_drive(
    Motor &left,
    Motor &right,
    PidContext &rotation)
{
    return (Drive){
        .left = left,
        .right = right,
        .rotation = rotation,
        .rotation_start_right = 0,
        .rotation_start_left = 0,
        .target_rotation = 0};
}

float drive_symetric_max_forward_speed(Drive &drive)
{
    auto left_max = motor_get_symetric_max_speed(drive.left);
    auto right_max = motor_get_symetric_max_speed(drive.right);
    return min(left_max, right_max);
}

float drive_symetric_min_forward_speed(Drive &drive)
{
    auto left_min = motor_get_symetric_min_speed(drive.left);
    auto right_min = motor_get_symetric_min_speed(drive.right);
    return max(left_min, right_min);
}

float drive_symetric_max_rotation_speed(Drive &drive)
{
    auto max_forward = drive_symetric_max_forward_speed(drive);
    return (max_forward * 2000) / WHEEL_DIAMETER;
}

float drive_symetric_min_rotation_speed(Drive &drive)
{
    auto min_forward = drive_symetric_min_forward_speed(drive);
    return (min_forward * 2000) / WHEEL_DIAMETER;
}

void drive_set_forward_speed(Drive &drive, float meters_per_seconds)
{
    pid_disable(drive.rotation);
    auto too_fast = abs(meters_per_seconds) > drive_symetric_max_forward_speed(drive);
    auto intentionnal_stop = meters_per_seconds == 0;
    auto too_slow = !intentionnal_stop && abs(meters_per_seconds) < drive_symetric_min_forward_speed(drive);
    if (too_slow || too_fast)
    {
        return; // ERR CASE, out of valid actuator bounds.
    }
    motor_set_speed(drive.left, meters_per_seconds);
    motor_set_speed(drive.right, meters_per_seconds);
}

void drive_set_rotation_speed(Drive &drive, float rad_per_seconds)
{
    auto too_fast = abs(rad_per_seconds) > drive_symetric_max_rotation_speed(drive);
    auto intentionnal_stop = rad_per_seconds == 0;
    auto too_slow = !intentionnal_stop && abs(rad_per_seconds) < drive_symetric_min_rotation_speed(drive);
    if (too_slow || too_fast)
    {
        return; // ERR CASE, out of valid actuator bounds.
    }
    auto wheel_speed = rad_per_seconds * WHEEL_DISTANCE / 2000.0f;
    motor_set_speed(drive.right, wheel_speed);
    motor_set_speed(drive.left, -wheel_speed);
}

void drive_set_rotate_by(Drive &drive, float rad)
{
    pid_reset(drive.rotation);
    drive.rotation_start_right = ENCODER_Read(RIGHT);
    drive.rotation_start_left = ENCODER_Read(LEFT);
    drive.rotation.setpoint = 0; // should always be 0 anyways, sanity check to make sure.
    drive.target_rotation = rad;
    drive.rotation.enabled = true;
}

float angle_travel(float source, float destination)
{
    // auto zeroed = destination - source;
    // if (zeroed > PI)
    // {
    //     return zeroed - PI * 2;
    // }
    // else if (zeroed < -PI)
    // {
    //     return zeroed + PI * 2;
    // }
    // else
    // {
    //     return zeroed;
    // }
    return atan2(
        sin(destination - source),
        cos(destination - source));
}

void drive_correct_power(Drive &drive, float dt_s)
{
    if (drive.rotation.enabled)
    {
        auto rotation_current_right = ENCODER_Read(RIGHT);
        auto rotation_current_left = ENCODER_Read(LEFT);
        auto rot_moved_right = drive.rotation_start_right - rotation_current_right;
        auto rot_moved_left = drive.rotation_start_left - rotation_current_left;

        auto rot_moved_right_mm = ticks_to_mm(WHEEL_TICK_PER_TURN, WHEEL_DIAMETER, rot_moved_right);
        auto rot_moved_left_mm = ticks_to_mm(WHEEL_TICK_PER_TURN, WHEEL_DIAMETER, rot_moved_left);

        auto circ = WHEEL_DISTANCE * PI;

        // auto angle_moved_right = (rot_moved_right_mm / circ) / (2 * PI);
        // auto angle_moved_left = (rot_moved_left_mm / circ) / (2 * PI);
        // auto avg_angle_moved = (angle_moved_left + angle_moved_right) / 2;
        auto angle_moved_right = rot_moved_right_mm / WHEEL_DISTANCE;
        auto angle_moved_left = -rot_moved_left_mm / WHEEL_DISTANCE;
        auto avg_angle_moved = (angle_moved_right + angle_moved_left) / 2.0f;

        auto angle_dist = angle_travel(avg_angle_moved, drive.target_rotation);
        auto output = pid_compute_next(drive.rotation, angle_dist, dt_s);
        drive_set_rotation_speed(drive, output);
    }
    motor_correct_power(drive.right, dt_s);
    motor_correct_power(drive.left, dt_s);
}

void drive_set_neutral(Drive &drive)
{
    motor_set_neutral(drive.right);
    motor_set_neutral(drive.left);
}