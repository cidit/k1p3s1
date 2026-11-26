#pragma once
#include <Arduino.h>
#include <LibRobus.h>
#include <stdlib.h>
#include <common/hardware.h>
#include <time.h>

namespace turret_aiming
{
  enum State
  {
    TURNING,
    DONE
  };

  constexpr float circumference = WHEEL_CIRCUMFERENCE / 1000; // m
  constexpr float dist_roue = WHEEL_DISTANCE / 1000.0;        // m
  constexpr float circle = dist_roue * 3.1416;                // m
  constexpr float kp = 0.002;                                 // 3b = 0.002 3a = 0.003
  constexpr float ki = 0.000001;
  constexpr float kd = 0.02;

  uint32_t _timer_last = 0, _timer_delay = 10; // millis
  State _state = DONE;

  float _target = 0, accumulated_error_0 = 0, accumulated_error_1 = 0, previous_error_0 = 0, previous_error_1= 0;
  int32_t _enco_offset_0 = 0, _enco_offset_1 = 0;

  float _last_pos = 0.0;
   #define PRINT(v) Serial.println(String(#v) + ": " + String(v));


  void rotate_to(float const_degrees)
  {
    _enco_offset_0 = ENCODER_Read(0);
    _enco_offset_1 = ENCODER_Read(1);
    float offset = 5.0 * (const_degrees / abs(const_degrees)); // 3b = 5.0 3a = 4.3
    _target = ((const_degrees + offset) / 360.0) * circle * (WHEEL_TICK_PER_TURN / circumference);
    accumulated_error_0 = 0; 
    accumulated_error_1 = 0;
    previous_error_0 = _target;
    previous_error_1 = _target;
    _state = TURNING;

    

    // PRINT(offset)
    // PRINT(_target)
    // PRINT(const_degrees)
    // PRINT(circle)
    // Serial.println("------------- ");
  }

  bool target_rotation_reached()
  {
    auto distance_0 = abs(_target - (ENCODER_Read(0) - _enco_offset_0));
    auto distance_1 = abs(_target + (ENCODER_Read(1) - _enco_offset_1));
    PRINT(distance_0)PRINT(distance_1)
    return distance_0 < 35 && distance_1 < 35;
  }

  void _update_pids_and_motors()
  {
    float error_0 = _target - (ENCODER_Read(0) - _enco_offset_0);
    float error_1 = _target + (ENCODER_Read(1) - _enco_offset_1);

    float derivative_0 = error_0 - previous_error_0;
    float derivative_1 = error_1 - previous_error_1;

    float speed_0 = (error_0 * kp) + (accumulated_error_0 * ki) + (derivative_0 * kd);
    float speed_1 = -((error_1 * kp) + (accumulated_error_1 * ki) + (derivative_1 * kd));

    if (speed_0 > 0.3)
      speed_0 = 0.3;
    else if (speed_0 < -0.3)
      speed_0 = -0.3;

    if (speed_1 > 0.3)
      speed_1 = 0.3;
    else if (speed_1 < -0.3)
      speed_1 = -0.3;

   
    // PRINT(speed_0)
    // PRINT(accumulated_error_0)
    // PRINT(error_0)


    MOTOR_SetSpeed(0, speed_0);
    MOTOR_SetSpeed(1, speed_1);
    accumulated_error_0 += error_0;
    accumulated_error_1 += error_1;
    previous_error_0 = error_0;
    previous_error_1 = error_1;
  }

  float _random_float_generator()
  {
    float n = (rand() % 201 - 100) / 100.0;
    while (_nearly_equal(n, 1.0) or _nearly_equal(n, -1.0)) {
      n = (rand() % 201 - 100) / 100.0;
    }


    return n;
  }

  bool _nearly_equal(float a, float b, float tolerance = 0.0001)
  {
    return fabs(a - b) < tolerance;
  }

  float _boundary_set(float last_pos, float increment)
  {
    return abs(increment) / increment - last_pos;
  }

  void random_turret_turn(float shooting_arc)
  {
    float increment = _random_float_generator();          // generates float between -1.0 and 1.0
    float boundary = _boundary_set(_last_pos, increment); // checks relative distance between last position and the boundary of the aim box in respect to the direction of the increment
    float new_aim = abs(boundary) * increment;
    float new_pos = _last_pos + new_aim; // calculates new position as the sum of the last position and and a proportion of the boundary
    float turn = new_aim * shooting_arc / 2;
    _last_pos = new_pos;
    rotate_to(turn);
  }

  void begin()
  {
    // nothing to do here!
  }

  void update()
  {
    if (_state == DONE)
    {
      return;
    }

    if (target_rotation_reached())
    {
      Serial.println("WE DONE YET?");
      _state = DONE;
      MOTOR_SetSpeed(0, 0);
      MOTOR_SetSpeed(1, 0);
      return;
    }

    auto now = millis();
    if (_timer_last + _timer_delay <= now)
    {
      _timer_last = now;
      _update_pids_and_motors();
    }
  }

}

//
// ----------------- IMPLÉMENTATION DE RÉFÉRENCE par Raph -----------------------------
//
// void _rotationPID(float degrees)
// {
//   float offset = 5.0 * (degrees / abs(degrees)); // 3b = 5.0 3a = 4.3
//   float target = ((degrees + offset) / 360.0) * circle * (WHEEL_TICK_PER_TURN / circumference);
//   float kp = 0.002; // 3b = 0.002 3a = 0.003
//   float ki = 0.000001;
//   float kd = 0.02;
//   float accumulated_error0 = 0.0;
//   float accumulated_error1 = 0.0;
//   ENCODER_Reset(0);
//   ENCODER_Reset(1);
//   float previous_error0 = target - ENCODER_Read(0);
//   float previous_error1 = target + ENCODER_Read(1);

//   while (abs(target - ENCODER_Read(0)) > 30 or abs(target + ENCODER_Read(1)) > 30)
//   {

//     float error0 = target - ENCODER_Read(0);
//     float error1 = target + ENCODER_Read(1);

//     float derivative0 = error0 - previous_error0;
//     float derivative1 = error1 - previous_error1;

//     float speed0 = (error0 * kp) + (accumulated_error0 * ki) + (derivative0 * kd);
//     float speed1 = -((error1 * kp) + (accumulated_error1 * ki) + (derivative1 * kd));

//     if (speed0 > 0.3)
//       speed0 = 0.3;
//     else if (speed0 < -0.3)
//       speed0 = -0.3;

//     if (speed1 > 0.3)
//       speed1 = 0.3;
//     else if (speed1 < -0.3)
//       speed1 = -0.3;

//     MOTOR_SetSpeed(0, speed0);
//     MOTOR_SetSpeed(1, speed1);
//     accumulated_error0 += error0;
//     accumulated_error1 += error1;
//     previous_error0 = error0;
//     previous_error1 = error1;
//     delay(10);
//   }
//   MOTOR_SetSpeed(0, 0);
//   MOTOR_SetSpeed(1, 0);
// }