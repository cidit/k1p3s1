#pragma once
#include "fonctions.h"
#include <LibRobus.h>
#include <robots\calib_3b_wheels.h>
#include <utils\calib.h>

void forwardcorrectionphil(float baseSpeed)
{
    float baseSpeed_left = lin_interpolate(left_wheel, baseSpeed); // technically not a basespeed but a power
    float baseSpeed_right = lin_interpolate(right_wheel, baseSpeed); // technically not a basespeed but a power
    static int32_t previous_count0 = 0;
    static int32_t previous_count1 = 0;

    float kp_correction = 0.005;

    int32_t count0 = ENCODER_Read(0);
    int32_t count1 = ENCODER_Read(1);

    int32_t delta0 = count0 - previous_count0;
    int32_t delta1 = count1 - previous_count1;

    previous_count0 = count0;
    previous_count1 = count1;

    float diff = (float)(delta0 - delta1);

    float correction = diff * kp_correction;

    MOTOR_SetSpeed(0, baseSpeed_left - correction);
    MOTOR_SetSpeed(1, baseSpeed_right + correction);
}
void rotationPIDphil(float degrees, float speed){
  float offset = 5.0*(degrees/abs(degrees));
  float target = ((degrees+offset)/360.0) * circle * (ticks_per_turn/circumference);

  float kp = 0.002;
  float ki = 0.000001;
  float kd = 0.02;

  float accumulated_error0 = 0.0;
  float accumulated_error1 = 0.0;

  ENCODER_Reset(0);
  ENCODER_Reset(1);

  float previous_error0 = target - ENCODER_Read(0);
  float previous_error1 = target + ENCODER_Read(1);

  while (abs(target - ENCODER_Read(0)) > 30 or abs(target + ENCODER_Read(1)) > 30){

    float error0 = target - ENCODER_Read(0);
    float error1 = target + ENCODER_Read(1);

    float derivative0 = error0 - previous_error0;
    float derivative1 = error1 - previous_error1;

    float speed0 = (error0 * kp) + (accumulated_error0 * ki) + (derivative0 * kd);
    float speed1 = -((error1 * kp) + (accumulated_error1 * ki) + (derivative1 * kd));

    // Limit motor speeds using the speed input
    if (speed0 > speed) speed0 = speed;
    else if (speed0 < -speed) speed0 = -speed;

    if (speed1 > speed) speed1 = speed;
    else if (speed1 < -speed) speed1 = -speed;

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
void forwardInMetersTrapezephil(float meters) {
  float it = 0;
  float target = (meters / circumference) * ticks_per_turn;
  float signe = target / abs(target);
  float kp_correction = 0.005; 
  float target_speed = 0.6;
  float speed = 0.2 * signe; // Initial speed based on direction


  ENCODER_Reset(0);
  ENCODER_Reset(1);
  unsigned long start = millis();

  while (abs(target)>abs((ENCODER_Read(0) + ENCODER_Read(1))/2)) {
    if(abs(speed) < abs(target_speed) and millis() - start >= 50 and abs((ENCODER_Read(0) + ENCODER_Read(1))/2) < abs(target) * 0.3){
      //Accélère
    speed +=0.02 + it * 0.005 * signe;
    start = millis();
    it ++;
    }
    else if(abs((ENCODER_Read(0) + ENCODER_Read(1))/2) > abs(target) * 0.6 and millis() - start >= 50 and abs(speed) > 0.2){
      speed -=(0.022/abs(meters)) * signe;
      start = millis();
    } 
    int32_t count_0 = ENCODER_Read(0);
    int32_t count_1 = ENCODER_Read(1);

    float diff = (float)(count_0 - count_1);
    float correction = diff * kp_correction;

    MOTOR_SetSpeed(0, speed- correction);
    MOTOR_SetSpeed(1, speed+ correction);
  }
  MOTOR_SetSpeed(0, 0);
  MOTOR_SetSpeed(1, 0); 
}

bool find_the_wall(float speed, int avant)
{
    int sensor = ROBUS_ReadIR(avant);
    Serial.println(sensor);

    forwardcorrectionphil(speed);

    if (ROBUS_ReadIR(avant) >= 310)
    {
        MOTOR_SetSpeed(0, 0);
        MOTOR_SetSpeed(1, 0);
        delay(10);

        rotationPIDphil(-90, 0.2);
        forwardInMetersTrapezephil(-0.5);

        MOTOR_SetSpeed(0, 0);
        MOTOR_SetSpeed(1, 0);
        delay(10);

        rotationPIDphil(90, 0.2);
        forwardInMetersTrapezephil(-0.5);

        MOTOR_SetSpeed(0, 0);
        MOTOR_SetSpeed(1, 0);
        delay(10);

        rotationPIDphil(90, 0.2);
        forwardInMetersTrapezephil(-0.5);

        MOTOR_SetSpeed(0, 0);
        MOTOR_SetSpeed(1, 0);
        delay(10);

        rotationPIDphil(-90, 0.2);

        return true;
    }

    return false;
}