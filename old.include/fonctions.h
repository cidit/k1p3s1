#pragma once

#include <Arduino.h>
#include <LibRobus.h>
#include <hardware.h>
#include <MegaServo/MegaServo.h>

const int pin_vert = 48;
const int pin_rouge = 49;
const int pin_ambiant = A0;
const int pin_5khz = A3;
const float circumference = 0.24; // m
const float dist_roue = (WHEEL_DISTANCE_OUTER - WHEEL_THICKNESS) / 1000.0; // m
const float circle = dist_roue*3.1416; // m
const float ticks_per_turn = 3200.0; //ticks
// const int _ir_receive_in = 37;

extern bool _robot_start;
extern bool _robot_wake_up;

void forwardInMeters(float meters);

void rotationPID(float degrees);
void finalDance(float degrees);
void turn_right();
void turn_left();
void turn_more_right();
void turn_more_left();

bool check_open();
void forwardInMetersTrapeze(float meters);

float difference_read(int pin_ambiant, int pin_5khz, int interval, int sample_size, int sample_time);
bool is_sifflet(int pin_ambiant, int pin_5khz, int patience, float integral_thr);

extern ColoredLED colored_leds[nb_leds];

void start_colored_leds(ColoredLED colored_leds[]);
void check_colored_leds(ColoredLED colored_leds[]);

float average_read(int pin_analog, int sample_size, int sample_time);
float difference(int pin_ambiant, int pin_5khz, int interval, int sample_size, int sample_time);
bool is_sifflet(/* EmaContext f_ambiant, EmaContext f_5khz, */int pin_ambiant, int pin_5khz, int patience, float thr, int interval); 
void reset_bool(bool bool_x[]);

// void beep(int times);
// void IR_control_check();
// bool can_robot_move();

bool find_the_wall(float speed,int pin);




extern MegaServo servo_up;
extern MegaServo servo_pince;


void raph_rotationPID(float degrees);
void raph_forwardInMetersTrapeze(float meters, int8_t back);
void pincer();
void lacher();
void verre();
void quille();
float ticks_to_deg(int ticks);



// fonctions.cpp
#include <LibRobus.h>
#include "fonctions.h"
#include "robots/calib_wheels.h"
#include "utils/calib.h"
#include "utils/ema.h"
#include "utils/misc.h"

bool _robot_start = false;
bool _robot_wake_up = true;

MegaServo servo_up;
MegaServo servo_pince;

void forwardInMeters(float meters)
{
  float offset = 0.0;
  float target = ((meters + offset) / circumference) * ticks_per_turn;

  float kp = 0.00006;
  float ki = 0.00000001;
  float kp_correction = 0.005;

  float accumulated_error0 = 0.0;
  float accumulated_error1 = 0.0;

  ENCODER_Reset(0);
  ENCODER_Reset(1);

  while (abs(target - ENCODER_Read(0)) > 10 or abs(target - ENCODER_Read(1)) > 10)
  {
    int32_t count_0 = ENCODER_Read(0);
    int32_t count_1 = ENCODER_Read(1);

    float error0 = target - count_0;
    float error1 = target - count_1;

    float speed0 = (error0 * kp) + (accumulated_error0 * ki);
    float speed1 = (error1 * kp) + (accumulated_error1 * ki);

    float diff = (float)(count_0 - count_1);
    float correction = diff * kp_correction;

    MOTOR_SetSpeed(0, speed0 - correction);
    MOTOR_SetSpeed(1, speed1 + correction);

    accumulated_error0 += error0;
    accumulated_error1 += error1;
  }

  MOTOR_SetSpeed(0, 0);
  MOTOR_SetSpeed(1, 0);
}
void rotationPID(float degrees)
{
  float offset = 5.0 * (degrees / abs(degrees)); // 3b = 5.0 3a = 4.3
  float target = ((degrees + offset) / 360.0) * circle * (ticks_per_turn / circumference);
  float kp = 0.002; // 3b = 0.002 3a = 0.003
  float ki = 0.000001;
  float kd = 0.02;
  float accumulated_error0 = 0.0;
  float accumulated_error1 = 0.0;
  ENCODER_Reset(0);
  ENCODER_Reset(1);
  float previous_error0 = target - ENCODER_Read(0);
  float previous_error1 = target + ENCODER_Read(1);

  while (abs(target - ENCODER_Read(0)) > 30 or abs(target + ENCODER_Read(1)) > 30)
  {

    float error0 = target - ENCODER_Read(0);
    float error1 = target + ENCODER_Read(1);

    float derivative0 = error0 - previous_error0;
    float derivative1 = error1 - previous_error1;

    float speed0 = (error0 * kp) + (accumulated_error0 * ki) + (derivative0 * kd);
    float speed1 = -((error1 * kp) + (accumulated_error1 * ki) + (derivative1 * kd));

    if (speed0 > 0.3)
      speed0 = 0.3;
    else if (speed0 < -0.3)
      speed0 = -0.3;

    if (speed1 > 0.3)
      speed1 = 0.3;
    else if (speed1 < -0.3)
      speed1 = -0.3;

    MOTOR_SetSpeed(0, speed0);
    MOTOR_SetSpeed(1, speed1);
    accumulated_error0 += error0;
    accumulated_error1 += error1;
    previous_error0 = error0;
    previous_error1 = error1;
    delay(10);
  }
  MOTOR_SetSpeed(0, 0);
  MOTOR_SetSpeed(1, 0);
}
void finalDance(float degrees)
{
  float target = (degrees / 360.0) * circle * (ticks_per_turn / circumference);

  float kp = 0.05;
  float kd = 0.01;
  float previous_error0 = target - ENCODER_Read(0);
  float previous_error1 = target + ENCODER_Read(1);

  ENCODER_Reset(0);
  ENCODER_Reset(1);
  while (abs(target - ENCODER_Read(0)) > 10 || abs(target + ENCODER_Read(1)) > 10)
  {

    float error0 = target - ENCODER_Read(0);
    float error1 = target + ENCODER_Read(1);

    float derivative0 = error0 - previous_error0;
    float derivative1 = error1 - previous_error1;

    float speed0 = (error0 * kp) + (derivative0 * kd);
    float speed1 = -((error1 * kp) + (derivative1 * kd));

    speed0 = constrain(speed0, -1.0, 1.0);
    speed1 = constrain(speed1, -1.0, 1.0);

    MOTOR_SetSpeed(0, speed0);
    MOTOR_SetSpeed(1, speed1);
  }

  MOTOR_SetSpeed(0, 0);
  MOTOR_SetSpeed(1, 0);
}
void turn_right()
{
  rotationPID(90);
}
void turn_left()
{
  rotationPID(-90);
}
void turn_more_right()
{
  rotationPID(91);
}
void turn_more_left()
{
  rotationPID(-91);
}

void forwardInMetersTrapeze(float meters)
{
  float it = 0;
  float target = (meters / circumference) * ticks_per_turn;
  float signe = target / abs(target);
  float kp_correction = 0.005;
  float target_speed = 0.6;
  float speed = 0.2 * signe; // Initial speed based on direction

  ENCODER_Reset(0);
  ENCODER_Reset(1);
  unsigned long start = millis();

  while (abs(target) > abs((ENCODER_Read(0) + ENCODER_Read(1)) / 2))
  {
    if (abs(speed) < abs(target_speed) and millis() - start >= 50 and abs((ENCODER_Read(0) + ENCODER_Read(1)) / 2) < abs(target) * 0.3)
    {
      // Accélère
      speed += 0.02 + it * 0.005 * signe;
      start = millis();
      it++;
    }
    else if (abs((ENCODER_Read(0) + ENCODER_Read(1)) / 2) > abs(target) * 0.6 and millis() - start >= 50 and abs(speed) > 0.2)
    {
      speed -= (0.022 / abs(meters)) * signe;
      start = millis();
    }
    int32_t count_0 = ENCODER_Read(0);
    int32_t count_1 = ENCODER_Read(1);

    float diff = (float)(count_0 - count_1);
    float correction = diff * kp_correction;

    MOTOR_SetSpeed(0, speed - correction);
    MOTOR_SetSpeed(1, speed + correction);
  }
  MOTOR_SetSpeed(0, 0);
  MOTOR_SetSpeed(1, 0);
}

bool check_open()
{
  if (digitalRead(pin_vert) and digitalRead(pin_rouge))
  {
    return true;
  }
  else
  {
    return false;
  }
}

void reset_bool(bool bool_x[])
{
  for (int i = 0; i < 3; i++)
  {
    if (i == 1)
      bool_x[i] = 1;
    else
      bool_x[i] = 0;
  }
}

float average_read(int pin_analog, int sample_size, int sample_time)
{
  float mean = 0;

  for (int i = 0; i < sample_size; i++)
  {
    int reading = analogRead(pin_analog);
    mean += reading / sample_size;
    delay(int(sample_time / 2));
  }
  return mean;
}

float difference(int pin_ambiant, int pin_5khz, int interval, int sample_size, int sample_time)
{
  float average_reading_ambiant = average_read(pin_ambiant, sample_size, sample_time);
  float average_reading_5khz = average_read(pin_5khz, sample_size, sample_time);

  float difference = average_reading_5khz - average_reading_ambiant;

  delay(interval);

  return difference;

  /*     Serial.println
      (
      String(">") +
      "avgA:" + String(average_reading_ambiant) + "," +
      "avg5:" + String(average_reading_5khz) + "," +
      "iA:" + String(integral_ambiant) + "," +
      "i5:" + String(integral_5khz) + "," +
      "id:" + String(integral_difference)
      );  */
}

bool is_sifflet(/* EmaContext f_ambiant, EmaContext f_5khz, */ int pin_ambiant, int pin_5khz, int patience, float thr, int interval)
{
  float sum = 0.0;
  for (int i = 0; i < patience; i++)
  {
    sum += difference(pin_ambiant, pin_5khz, interval, 5, 10);
    // sum += difference_ema(f_ambiant, f_5khz);
    if (sum > thr / patience * i)
    {
      return true;
    }
  }
  // Serial.println("false");
  return false;
}
ColoredLED colored_leds[nb_leds] = {
    ColoredLED(38),
    ColoredLED(39),
    ColoredLED(40),
    ColoredLED(41),
    ColoredLED(42)};


// void IR_control_check()
// {
//   switch (REMOTE_read())
//   {
//   case 0xF70812ED:
//     Serial.println("Start command received");
//     beep(2);
//     _robot_start = true;
//     break;
//   case 0x778812ED:
//     Serial.println("Wake command received");
//     beep(3);
//     _robot_wake_up = true;
//     break;
//   }
// }

// bool can_robot_move()
// {
//   IR_control_check();
//   return _robot_start and _robot_wake_up;
// }

void raph_rotationPID(float degrees)
{
  float offset = 5.0 * (degrees / abs(degrees)); // 3b = 5.0 3a = 4.3
  float target = ((degrees + offset) / 360.0) * circle * (ticks_per_turn / circumference);
  float kp = 0.002; // 3b = 0.002 3a = 0.003
  float ki = 0.000001;
  float kd = 0.02;
  float accumulated_error0 = 0.0;
  float accumulated_error1 = 0.0;
  ENCODER_Reset(0);
  ENCODER_Reset(1);
  float previous_error0 = target - ENCODER_Read(0);
  float previous_error1 = target + ENCODER_Read(1);

  while (abs(target - ENCODER_Read(0)) > 100 or abs(target + ENCODER_Read(1)) > 100)
  {

    float error0 = target - ENCODER_Read(0);
    float error1 = target + ENCODER_Read(1);

    float derivative0 = error0 - previous_error0;
    float derivative1 = error1 - previous_error1;

    float speed0 = (error0 * kp) + (accumulated_error0 * ki) + (derivative0 * kd);
    float speed1 = -((error1 * kp) + (accumulated_error1 * ki) + (derivative1 * kd));

    if (speed0 > 0.3)
      speed0 = 0.3;
    else if (speed0 < -0.3)
      speed0 = -0.3;

    if (speed1 > 0.3)
      speed1 = 0.3;
    else if (speed1 < -0.3)
      speed1 = -0.3;

    MOTOR_SetSpeed(0, speed0);
    MOTOR_SetSpeed(1, speed1);
    accumulated_error0 += error0;
    accumulated_error1 += error1;
    previous_error0 = error0;
    previous_error1 = error1;
    delay(10);
  }
  MOTOR_SetSpeed(0, 0);
  MOTOR_SetSpeed(1, 0);
}

void raph_forwardInMetersTrapeze(float meters, int8_t back)
{
  float it = 0;
  float target = back * (meters / circumference) * ticks_per_turn;
  float signe = (target != 0) ? (target / abs(target)) : 1.0;
  float kp_correction = 0.005;
  float target_speed = back * 0.3;
  float speed = 0.2 * signe; // Vitesse initiale orientée selon la direction

  ENCODER_Reset(0);
  ENCODER_Reset(1);
  unsigned long start = millis();

  while (abs(target) > abs((ENCODER_Read(0) + ENCODER_Read(1)) / 2))
  {
    // Accélération (Phase 1)
    if (abs(speed) < abs(target_speed) && millis() - start >= 50 && abs((ENCODER_Read(0) + ENCODER_Read(1)) / 2) < abs(target) * 0.3)
    {
      speed += (0.02 + (it * 0.005)) * signe;
      start = millis();
      it++;
    }
    // Décélération (Phase 3)
    else if (abs((ENCODER_Read(0) + ENCODER_Read(1)) / 2) > abs(target) * 0.6 && millis() - start >= 50 && abs(speed) > 0.2)
    {
      speed -= (0.022 / abs(meters)) * signe;
      start = millis();
    }

    int32_t count_0 = ENCODER_Read(0);
    int32_t count_1 = ENCODER_Read(1);

    float diff = (float)(count_0 - count_1);
    float correction = diff * kp_correction;

    // Application des vitesses corrigées sur les moteurs
    MOTOR_SetSpeed(0, speed - correction);
    MOTOR_SetSpeed(1, speed + correction);
  }

  // Arrêt des moteurs à la fin
  MOTOR_SetSpeed(0, 0);
  MOTOR_SetSpeed(1, 0);
}

void pincer()
{
  servo_pince.write(100);
  delay(2000);
  servo_up.write(80);
  delay(2000);
}

void lacher()
{
  servo_up.write(120);
  delay(2000);
  servo_pince.write(170);
  delay(2000);
}

void verre()
{
  // Setup pince
  servo_pince.write(150);
  servo_up.write(120);

  int treshold = 200;
  unsigned long last_sample_time = 0;
  EmaContext moving_avg = make_EMA(T_from_Hz(100), 0.1);
  int offset = -15;

  ENCODER_Reset(0);
  ENCODER_Reset(1);

  MOTOR_SetSpeed(0, 0.2);
  MOTOR_SetSpeed(1, -0.2);

  bool exit = false;
  while (exit == false)
  {
    if (millis() - last_sample_time >= 10)
    {
      last_sample_time = millis();
      EMA_add(moving_avg, ROBUS_ReadIR(1));
      GRAPH(moving_avg.current);
    }

    if (moving_avg.current > treshold)
    {
      MOTOR_SetSpeed(0, 0);
      MOTOR_SetSpeed(1, 0);

      int16_t tick_turn = (abs(ENCODER_Read(0)) + abs(ENCODER_Read(1))) / 2;
      raph_rotationPID(offset);

      Serial.println("Dedans ! ");
      delay(200);
      Serial.println("Tourne");
      raph_rotationPID(180);

      raph_forwardInMetersTrapeze(0.4, 1);
      Serial.println("fin");
      delay(200);
      pincer();
      delay(200);

      raph_forwardInMetersTrapeze(0.4, -1);

      Serial.print("Degrees naturel: ");

      float degrees = int(ticks_to_deg(tick_turn));
      Serial.println(degrees);

      Serial.print("Degrees : ");
      int deg = int(degrees) % 360 + offset + 180;
      Serial.println(deg);

      if (deg > 180)
      {
        raph_rotationPID(360 - deg);
      }
      else
      {
        raph_rotationPID(-deg);
      }

      exit = true;
    }
  }
}

void quille()
{
  int treshold = 200;
  unsigned long last_sample_time = 0;
  EmaContext moving_avg = make_EMA(T_from_Hz(100), 0.1);
  int offset = -15;

  ENCODER_Reset(0);
  ENCODER_Reset(1);

  MOTOR_SetSpeed(0, 0.2);
  MOTOR_SetSpeed(1, -0.2);

  bool exit = false;
  while (exit == false)
  {
    if (millis() - last_sample_time >= 10)
    {
      last_sample_time = millis();
      EMA_add(moving_avg, ROBUS_ReadIR(1));
      GRAPH(moving_avg.current);
    }

    if (moving_avg.current > treshold)
    {
      MOTOR_SetSpeed(0, 0);
      MOTOR_SetSpeed(1, 0);

      int16_t tick_turn = (abs(ENCODER_Read(0)) + abs(ENCODER_Read(1))) / 2;

      raph_rotationPID(offset);

      Serial.println("Dedans ! ");
      delay(200);
      raph_forwardInMetersTrapeze(0.4, -1);
      delay(250);

      raph_forwardInMetersTrapeze(0.4, 1);

      Serial.print("Degrees naturel: ");
      float degrees = ticks_to_deg(tick_turn);
      Serial.println(degrees);

      Serial.print("Degrees : ");
      int deg = int(degrees) % 360 + offset;
      Serial.println(deg);

      if (deg > 180)
      {
        raph_rotationPID(360 - deg);
      }
      else
      {
        raph_rotationPID(-deg);
      }
      exit = true;
    }
  }
}

float ticks_to_deg(int ticks)
{
  return ticks * (circumference / ticks_per_turn) * (360 / circle);
}