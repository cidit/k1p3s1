#include <Arduino.h>
#include <LibRobus.h>
#include <Decodeur.h>

#include "utils/misc.h"
#include "utils/timer.h"
#include "utils/ring_buffer.h"
#include "utils/pid.h"
#include "utils/ema.h"
#include "utils/task.h"

#include "hardware.h"
#include "odometry2wheels.h"
#include "drive.h"

#include "tasks/calibration.h"
#include "tasks/track_speed.h"
#include "tasks/update_drive.h"

#define ROBOT_3B

#ifdef ROBOT_3A
#include "robots/calib_3a_wheels.h"
#else
#include "robots/calib_3b_wheels.h"
#endif

Decodeur input(&Serial);

TimerContext graph_timer = make_timer(us_from_s(T_from_Hz(2)), 0);
TimerContext encoder_sampling = make_timer(us_from_s(T_from_Hz(200)), 0);
TimerContext pid_cycle = make_timer(us_from_s(T_from_Hz(80)), 0);

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
PidContext rotation_pid = make_pid(PidK{.p = 0.2, .i = 0, .d = 0}, D_ON_PV);
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

MAKE_TASK(CALIBRATION, right_speed_ema, left_speed_ema)
MAKE_TASK(TRACK_SPEED, encoder_sampling, right_enco, left_enco, right_speed_ema, left_speed_ema, odometry)
MAKE_TASK(UPDATE_DRIVE, pid_cycle, drive)

// MAKE_TASK_NAMED(TRACK_MOVEMENT_RIGHT, TRACK_MOVEMENT,
//                 encoder_sampling, right_enco)
// MAKE_TASK_NAMED(TRACK_MOVEMENT_LEFT, TRACK_MOVEMENT,
//                 encoder_sampling, left_enco)
// MAKE_TASK_NAMED(TRACK_SPEED_RIGHT, TRACK_SPEED,
//                 encoder_sampling, right_enco, right_speed_ema)
// MAKE_TASK_NAMED(TRACK_SPEED_LEFT, TRACK_SPEED,
//                 encoder_sampling, left_enco, left_speed_ema)
// MAKE_TASK(TRACK_ODOMETRY,
//           encoder_sampling, right_enco, left_enco, odometry)
// MAKE_TASK(UPDATE_DRIVE,
//           pid_cycle, drive)
// MAKE_TASK(CALIBRATION, right_speed_ema, left_speed_ema)

Task *background_tasks[] = {
    &TRACK_SPEED_TASK,
    &UPDATE_DRIVE_TASK,
};

Task *tasks[] = {
    // &CALIBRATION_TASK,
};

bool command_write_float(String key, float value)
{

#define EXPOSE_VAL(k) \
  if (key == #k)      \
  {                   \
    k = value;        \
    return true;      \
  }

  EXPOSE_VAL(right_pid.k.p)
  EXPOSE_VAL(right_pid.k.i)
  EXPOSE_VAL(right_pid.k.d)
  EXPOSE_VAL(left_pid.k.p)
  EXPOSE_VAL(left_pid.k.i)
  EXPOSE_VAL(left_pid.k.d)
  Serial.println("not found");
  return false;
}

bool command_read_float(String key)
{
#define EXPOSE_VAL(k)  \
  if (key == #k)       \
  {                    \
    Serial.println(k); \
    return true;       \
  }

  EXPOSE_VAL(right_pid.k.p)
  EXPOSE_VAL(right_pid.k.i)
  EXPOSE_VAL(right_pid.k.d)
  EXPOSE_VAL(left_pid.k.p)
  EXPOSE_VAL(left_pid.k.i)
  EXPOSE_VAL(left_pid.k.d)
  Serial.println("not found");
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

  if (cmd == "help")
  {
    Serial.println("available commands:");
    Serial.println("\twrite");
    Serial.println("\tread");
    Serial.println("\ttask");
    Serial.println("\tcall");
  }
  else if (cmd == "write")
  {
    // change in-memory values
    for (auto i = 0; i < nb_args; i++)
    {
      auto arg = input.getArgString(i);
      auto pos_eq = arg.indexOf("=");
      if (pos_eq == -1)
      {
        Serial.println("missing equal (=)");
      }
      auto key = arg.substring(0, pos_eq);
      auto value = arg.substring(pos_eq + 1).toFloat();
      command_write_float(key, value);
    }
  }
  else if (cmd == "read")
  {
    // read in-memory values
    for (auto i = 0; i < nb_args; i++)
    {
      auto arg = input.getArgString(i);
      command_read_float(arg);
    }
  }
  else if (cmd == "task")
  {
    auto taskname = input.getArgString(0);
    if (taskname == "CALIB")
    {
      motor_set_neutral(drive.left);
      motor_set_neutral(drive.right);
      if (CALIBRATION_TASK.state == DONE)
      {
        CALIBRATION_TASK.state = SETUP;
      }
      else
      {
        CALIBRATION_TASK.state = DONE;
      }
    } 
    else {
      Serial.println("not found");
    }
  }
  else if (cmd == "call")
  {
    auto function_name = input.getArgString(0);
    if (function_name == "MOTOR_SetSpeed")
    {
      MOTOR_SetSpeed(input.getArg(1), input.getArg(2));
    }
    else if (function_name == "motor_set_speed")
    {
      auto &motor = input.getArg(1) == RIGHT ? drive.right : drive.left;
      auto speed = input.getArg(2);
      motor_set_speed(motor, speed);
    }
    else if (function_name == "motor_set_neutral")
    {
      auto &motor = input.getArg(1) == RIGHT ? drive.right : drive.left;
      motor_set_neutral(motor);
    }
    else if (function_name == "drive_set_forward_speed")
    {
      auto speed = input.getArg(1);
      drive_set_forward_speed(drive, speed);
    }
    else {
      Serial.println("not found");
    }
  }
  else {
    Serial.println("not found");
  }
}

void setup()
{
  BoardInit();
  Serial.begin(115200);

  // begin background tasks;
  for (auto task : background_tasks)
  {
    task->state = TaskState::SETUP;
  }
}

void loop()
{
  handle_serial_input();

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

  auto now = micros();
  if (timer_ready(graph_timer, now) && true)
  {
    timer_reset(graph_timer, now);

    Serial.println(">" + String("AX_GetVoltage()") + ':' + String(millis()) + ':' + String(AX_GetVoltage()));

    if (right_speed_ema.ready && left_speed_ema.ready)
    {
      auto r_mps = right_speed_ema.current;
      auto l_mps = left_speed_ema.current;

      GRAPHT(r_mps)
      GRAPHT(l_mps)
      GRAPHT(r_mps / l_mps)
      GRAPH(right_motor.applied_power)
      
      auto rot = odometry.orientation_rad;
      GRAPHT(rot)
      
      PLOT("odometry", odometry.position_mm.x, odometry.position_mm.y)
      
      Serial.println();
      Serial.println(r_mps, 5);
      Serial.println(l_mps, 5);
      Serial.println(r_mps / l_mps, 5);
    }
  }
}