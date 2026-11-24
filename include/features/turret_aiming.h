#include <Arduino.h>
#include <LibRobus.h>
#include <stdlib.h>
#include <time.h>

namespace turret_aiming {
const float WHEEL_DIAMETER = 76.5; // mm
const float WHEEL_DISTANCE_OUTER = 190.0; // mm
const float WHEEL_THICKNESS = 6.65; // mm
const float WHEEL_TICK_PER_TURN = 3200.0;
const float WHEEL_DISTANCE = WHEEL_DISTANCE_OUTER - WHEEL_THICKNESS;
const float circumference = 0.24; // m
const float dist_roue = (WHEEL_DISTANCE_OUTER - WHEEL_THICKNESS) / 1000.0; // m
const float circle = dist_roue*3.1416; // m
const float ticks_per_turn = 3200.0; //ticks

float _last_pos = 0.0;

void _rotationPID(float degrees)
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


float _random_float_generator() {
    float n = (rand() % 201 - 100) / 100.0;
    return n;
}

bool _nearly_equal(float a, float b, float tolerance = 0.0001) {
    return fabs(a - b) < tolerance;
}

float _boundary_set(float last_pos, float increment) {
    return abs(increment) / increment - last_pos;
}

void turret_turns(float shooting_arc) {
    float increment = _random_float_generator();  // generates float between -1.0 and 1.0
    float boundary = _boundary_set(_last_pos, increment); // checks relative distance between last position and the boundary of the aim box in respect to the direction of the increment
    float new_aim = abs(boundary) * increment;
    float new_pos = _last_pos + new_aim; // calculates new position as the sum of the last position and and a proportion of the boundary
    float turn = new_aim * shooting_arc / 2;
    last_pos = new_pos;
    _rotationPID(turn);
}

}