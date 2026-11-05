#include <Arduino.h>
#include <LibRobus.h>

#include "utils/misc.h"
#include "utils/timer.h"
#include "utils/ring_buffer.h"
#include "utils/pid.h"
#include "utils/ema.h"
#include "utils/task.h"

#include "hardware.h"
#include "odometry2wheels.h"
#include "drive.h"
#include "line_follower.h"

#include "tasks/idle.h"
#include "tasks/track_line.h"
#include "tasks/track_speed.h"
#include "tasks/update_drive.h"
#include "tasks/detect_color.h"
#include "tasks/epreuve_rose.h"
#include "tasks/epreuve_orange.h"
#include "tasks/epreuve_jaune.h" // INUTILE
#include "tasks/epreuve_verte.h"
#include "tasks/epreuve_bleue.h"
#include "tasks/check_tasks.h"

#include "epreuve_jaune.h"

#define ROBOT_3A

#include "robots/calib_wheels.h"

TimerContext graph_timer = make_timer(us_from_s(T_from_Hz(2)), 0);
TimerContext encoder_sampling = make_timer(us_from_s(T_from_Hz(200)), 0);
TimerContext pid_cycle = make_timer(us_from_s(T_from_Hz(80)), 0);
TimerContext line_follower_timer = make_timer(us_from_s(T_from_Hz(50)), 0);

RingBuffer<2> right_enco = make_ring_buffer<2>();
RingBuffer<2> left_enco = make_ring_buffer<2>();

EmaContext right_speed_ema = make_EMA(encoder_sampling.period, .1);
EmaContext left_speed_ema = make_EMA(encoder_sampling.period, .1);

OdometryContext odometry = make_odometry();

// bool to complete tasks
bool blue_done = false;
bool pink_done = false;
bool yellow_done = false;
bool green_done = false;
bool orange_done = false;


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

uint8_t follower_pins[] = {A2, A3, A4};
constexpr size_t follower_pins_len = sizeof(follower_pins) / sizeof(follower_pins[0]);
EmaContext follower_emas[follower_pins_len];

MappingParameters mappings[follower_pins_len] = {
    {.min = 485, .max = 693},
    {.min = 464, .max = 810},
    {.min = 569, .max = 850},
};

int couleur;
uint16_t red, green, blue;

MAKE_TASK(IDLE)
MAKE_TASK(TRACK_SPEED, encoder_sampling, right_enco, left_enco, right_speed_ema, left_speed_ema, odometry)
MAKE_TASK(UPDATE_DRIVE, pid_cycle, drive)
MAKE_TASK(TRACK_LINE, line_follower_timer, follower_pins, mappings, follower_emas)
MAKE_TASK(detect_color, couleur, red, green, blue)
MAKE_TASK(epreuve_rose, pink_done)
MAKE_TASK(epreuve_orange, orange_done)
MAKE_TASK(epreuve_jaune, yellow_done)
MAKE_TASK(epreuve_verte, green_done)
MAKE_TASK(epreuve_bleue, blue_done)
MAKE_TASK(CHECK_COLORS_DONE, blue_done, pink_done, yellow_done, green_done, orange_done)

Task *background_tasks[] = {
    &TRACK_SPEED_TASK,
    &UPDATE_DRIVE_TASK,
    &TRACK_LINE_TASK,
    &CHECK_COLORS_DONE_TASK,
    &detect_color_TASK};

Task *current_task = &IDLE_TASK;

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
  // begin background tasks;
  for (auto task : background_tasks)
  {
    task->state = TaskState::SETUP;
  }
}

void loop()
{
  /* ===== TASK RUNNING ===== */

  for (auto task : background_tasks)
  {
    task_run(*task);
  }
  if (current_task->state != DONE)
  {
    task_run(*current_task);
  }
  else
  {
    current_task = &IDLE_TASK;
  }

  /* ===== LINE FOLLOWER ===== */

  float follower_vals[] = {
      follower_emas[0].current,
      follower_emas[1].current,
      follower_emas[2].current,
  };
  auto intensity_ = intensity(follower_vals, follower_pins_len);
  auto distribution_ = distribution(follower_vals, follower_pins_len);

  auto base_speed = 0.15;
  // motor_set_speed(drive.left, -(base_speed - distribution_ / 4));
  // motor_set_speed(drive.right, -(base_speed + distribution_ / 4));

  /* ===== LANCEMENT DES EPREUVES ==== */

  if (current_task->state == DONE)
  {
    if (couleur == ROSE)
    {
      epreuve_rose_TASK.state = SETUP;
      current_task = &epreuve_rose_TASK;
    }
    else if (couleur == ORANGE)
    { 
      epreuve_orange_TASK.state = SETUP;
      current_task = &epreuve_orange_TASK;
    }
    else if (couleur == JAUNE)
    {
      // epreuve_jaune_TASK.state = SETUP;
      // current_task = &epreuve_jaune_TASK;
      pid_disable(drive.left.speed_correction);
      pid_disable(drive.right.speed_correction);
      pid_disable(drive.rotation);
      find_the_wall(-0.5, 1);
      yellow_done = true;
      couleur = UNKNOWN_COLOR;
    }
    else if (couleur == VERT)
    {
      epreuve_verte_TASK.state = SETUP;
      current_task = &epreuve_verte_TASK;
    }
    else if (couleur == BLEU)
    {
      epreuve_bleue_TASK.state = SETUP;
      current_task = &epreuve_bleue_TASK;
    }
  }

  /* ===== PRINTIGNT SECTION ==== */

  auto now = micros();
  if (timer_ready(graph_timer, now) && false)
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