#include <Arduino.h>
#include <LibRobus.h>

#include <utils/misc.h>
#include <utils/task.h>
#include <utils/timer.h>

#include <line_follower.h>

#include <tasks/track_speed.h>
#include <tasks/update_drive.h>
#include <tasks/track_line.h>

#define ROBOT_3A

#include "robots/calib_wheels.h"

TimerContext graph_timer = make_timer(us_from_s(T_from_Hz(20)), 0);
TimerContext encoder_sampling = make_timer(us_from_s(T_from_Hz(200)), 0);
TimerContext pid_cycle = make_timer(us_from_s(T_from_Hz(80)), 0);
TimerContext line_follower_timer = make_timer(us_from_s(T_from_Hz(50)), 0);

RingBuffer<2> right_enco = make_ring_buffer<2>();
RingBuffer<2> left_enco = make_ring_buffer<2>();

EmaContext right_speed_ema = make_EMA(encoder_sampling.period, .1);
EmaContext left_speed_ema = make_EMA(encoder_sampling.period, .1);

OdometryContext odometry = make_odometry();

#ifdef ROBOT_3A
PidContext right_pid = make_pid(PidK{.p = 0.2, .i = 0, .d = 0}, D_ON_PV);
PidContext left_pid = make_pid(PidK{.p = 0.2, .i = 0, .d = 0}, D_ON_PV);
PidContext rotation_pid = make_pid(PidK{.p = 0.2, .i = 0, .d = 0}, D_ON_PV);
#else
// PidContext right_pid = make_pid(PidK{.p = 1.3, .i = 0.01, .d = 0}, D_ON_PV);
PidContext right_pid = make_pid(PidK{.p = 0.3, .i = 0, .d = 0}, D_ON_PV);
PidContext left_pid = make_pid(PidK{.p = 0.2, .i = 0, .d = 0}, D_ON_PV);
#endif

Motor right_motor = make_motor(RIGHT,
                               false,
                               right_pid,
                               right_speed_ema,
                               right_wheel);

Motor left_motor = make_motor(LEFT,
                              false,
                              left_pid,
                              left_speed_ema,
                              left_wheel);

Drive drive = make_drive(left_motor, right_motor, rotation_pid);

uint8_t follower_pins[] = {A2, A3, A4};
constexpr size_t follower_pins_len = sizeof(follower_pins) / sizeof(follower_pins[0]);
EmaContext follower_emas[follower_pins_len];

MappingParameters mappings[follower_pins_len] = {
    {.min = 485, .max = 693},
    {.min = 464, .max = 810},
    {.min = 569, .max = 850},
};

MAKE_TASK(TRACK_SPEED, encoder_sampling, right_enco, left_enco, right_speed_ema, left_speed_ema, odometry)
MAKE_TASK(UPDATE_DRIVE, pid_cycle, drive)
MAKE_TASK(TRACK_LINE, line_follower_timer, follower_pins, mappings, follower_emas)

Task *background_tasks[] = {
    &TRACK_SPEED_TASK,
    &UPDATE_DRIVE_TASK,
    &TRACK_LINE_TASK,
};

Task *tasks[] = {
    // &CALIBRATION_TASK,
};

void setup()
{
    BoardInit();
    Serial.begin(115200);

    for (auto p : follower_pins)
    {
        pinMode(p, INPUT);
    }
    for (size_t i = 0; i < follower_pins_len; i++)
    {
        follower_emas[i] = make_EMA(s_from_us(line_follower_timer.period), 0.05);
    }
    for (auto task : background_tasks)
    {
        task->state = TaskState::SETUP;
    }
}

void loop()
{
    for (auto task : background_tasks)
    {
        task_run(*task);
    }
    for (auto task : tasks)
    {
        if (task->state != DONE)
        {
            task_run(*task);
        }
    }

    float follower_vals[] = {
        follower_emas[0].current,
        follower_emas[1].current,
        follower_emas[2].current,
    };
    auto intensity_ = intensity(follower_vals, follower_pins_len);
    auto distribution_ = distribution(follower_vals, follower_pins_len);

    auto base_speed = 0.15;
    motor_set_speed(drive.left, -(base_speed - distribution_ / 4));
    motor_set_speed(drive.right, -(base_speed + distribution_ / 4));

    auto now_micros = micros();
    if (timer_ready(graph_timer, now_micros))
    {
        timer_reset(graph_timer, now_micros);

        GRAPH(follower_emas[0].current)
        GRAPH(follower_emas[1].current)
        GRAPH(follower_emas[2].current)
        GRAPH(intensity_)
        GRAPH(distribution_)
    }
}