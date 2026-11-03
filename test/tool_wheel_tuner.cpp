#include <Arduino.h>
#include <LibRobus.h>
#include <Decodeur.h>

#include "utils/misc.h"
#include "utils/timer.h"
#include "utils/ring_buffer.h"
#include "utils/pid.h"
#include "utils/ema.h"
#include "utils/task.h"
#include "drive.h"
#include "hardware.h"
#include "odometry2wheels.h"

#include "tasks/idle.h"
#include "tasks/track_speed.h"
#include "tasks/calibration.h"

// auto something = 3b_wheels_left_speed;

Decodeur input(&Serial);

TimerContext graph_timer = make_timer(us_from_s(T_from_Hz(2)), 0);
TimerContext encoder_sampling = make_timer(us_from_s(T_from_Hz(500)), 0);
TimerContext pid_cycle = make_timer(us_from_s(T_from_Hz(100)), 0);

RingBuffer<2> right_enco = make_ring_buffer<2>();
RingBuffer<2> left_enco = make_ring_buffer<2>();

EmaContext right_speed = make_EMA(encoder_sampling.period, .01);
EmaContext left_speed = make_EMA(encoder_sampling.period, .01);

OdometryContext odometry = make_odometry();

PidContext some_exemple_pid = make_pid(PidK{.p = 0, .i = 0, .d = 0}, D_ON_PV);
PidContext right_speed_pid = make_pid(PidK{.p = 0, .i = 0, .d = 0}, D_ON_PV);

// Drive drive = Drive {};

MAKE_TASK(IDLE)
MAKE_TASK(CALIBRATION, right_speed, left_speed)

MAKE_TASK(TRACK_SPEED, encoder_sampling, right_enco, left_enco, right_speed, left_speed, odometry)

Task current_task = IDLE_TASK;

Task allways_on_background_tasks[] = {
    TRACK_SPEED_TASK};

bool command_write_float(String key, float value)
{

#define EXPOSE_VAL(k) \
  if (key == #k)      \
  {                   \
    k = value;        \
    return true;      \
  }

  EXPOSE_VAL(some_exemple_pid.k.p)
  EXPOSE_VAL(some_exemple_pid.k.i)
  EXPOSE_VAL(some_exemple_pid.k.d)
  return false;
}

void handle_serial_input()
{
  input.refresh();

  if (!input.isAvailable())
  {
    return;
  }

  auto cmd = input.getCommandString();
  auto nb_args = input.getArgCount();

  if (cmd == "write")
  {
    // change in-memory values
    for (auto i = 0; i < nb_args; i++)
    {
      auto arg = input.getArgString(i);
      auto pos_eq = arg.indexOf("=");
      auto key = arg.substring(0, pos_eq);
      auto value = arg.substring(pos_eq + 1).toFloat();
      command_write_float(key, value);
    }
  }
  // else if (cmd == "set")
  // {
  //   // change properties of the system.
  // }
  else if (cmd == "task")
  {
    // add a task to the queue.
    auto taskname = input.getArgString(0);
    if (taskname == "CALIB")
    {
      current_task = CALIBRATION_TASK;
    }
  }
  else if (cmd == "call")
  {
    // call a function..?
    if (input.getArgString(0) == "MOTOR_SetSpeed")
    {
      MOTOR_SetSpeed(input.getArg(1), input.getArg(2));
    }
  }
}

void setup()
{
  BoardInit();
  Serial.begin(115200);
}

void loop()
{
  auto now = micros();
  handle_serial_input();

  for (auto &task : allways_on_background_tasks)
  {
    run_task(task);
  }

  run_task(current_task);
  if (current_task.state == DONE)
  {
    current_task = IDLE_TASK;
  }

  if (timer_ready(graph_timer, now) && true)
  {
    timer_reset(graph_timer, now);

    GRAPHT(AX_GetVoltage())

    if (right_speed.ready && left_speed.ready)
    {
      auto r_mps = right_speed.current;
      auto l_mps = left_speed.current;

      GRAPHT(r_mps)
      GRAPHT(l_mps)
      GRAPHT(r_mps / l_mps)

      auto rot = odometry.orientation_rad;
      GRAPHT(rot)

      PLOT("odometry", odometry.position_mm.x, odometry.position_mm.y)
    }
  }
}

/*
// NOTES:
1. keep track of whether or not the wheels are spinning (torqued). if not, apply an initial torque of
max voltage for a single loop, ***just to kick it out of the static state where the friction
coeficient is highest***

- KICKSTART boolean field on the motor struct.
  set kickstart at true and voltage at max when applying a speed.
  set it back to false when we detect an encoder change and apply the
    actually requested voltage instead.
*/