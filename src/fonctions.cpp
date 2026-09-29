#include "fonctions.h"
#include <LibRobus.h>


void forwardInMeters(float meters) {
  float target = (meters / circumference) * ticks_per_turn;
  Serial.print("Target ticks: ");
  Serial.println(target);

  float kp = 0.00005;
  float ki = 0.00000001;
  float kp_correction = 0.005; 

  float accumulated_error0 = 0.0;
  float accumulated_error1 = 0.0;
  

  ENCODER_Reset(0);
  ENCODER_Reset(1);

  while (abs(target - ENCODER_Read(0)) > 30 || abs(target - ENCODER_Read(1)) > 30) {
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

void rotationPID(float degrees){
  float target = (degrees/360.0) * circle * (ticks_per_turn/circumference);
  float kp = 0.0001;
  float ki = 0.00000975;//0.00000945
  float kd = 0.0008;
  float accumulated_error0 = 0.0;
  float accumulated_error1 = 0.0;
  ENCODER_Reset(0);
  ENCODER_Reset(1);
  float previous_error0 = target - ENCODER_Read(0);
  float previous_error1 = target + ENCODER_Read(1);
  Serial.print("target : ");
  Serial.println(target);
  
  while (abs(target - ENCODER_Read(0)) > 30 && abs(target + ENCODER_Read(1)) > 30){
  Serial.print("ERROR 0 : ");
  Serial.println(target - ENCODER_Read(0));
  Serial.print("ERROR 1 : ");
  Serial.println(target + ENCODER_Read(1));

  float error0 = target - ENCODER_Read(0);
  float error1 = target + ENCODER_Read(1);

  float derivative0 = error0 - previous_error0;
  float derivative1 = error1 - previous_error1;
  
  float speed0 = (error0 * kp) + (accumulated_error0 * ki) + (derivative0 * kd);
  float speed1 = -((error1 * kp) + (accumulated_error1 * ki) + (derivative1 * kd));

  MOTOR_SetSpeed(0, speed0);
  MOTOR_SetSpeed(1, speed1);
  accumulated_error0 += error0;
  accumulated_error1 += error1;
  previous_error0 = error0;
  previous_error1 = error1;
  }
  MOTOR_SetSpeed(0, 0);
  MOTOR_SetSpeed(1, 0);
}
void turn_right(){
  rotationPID(90);
}
void turn_left(){
  rotationPID(-90);
}
bool check_open(){
    if (digitalRead(pin_vert) or digitalRead(pin_rouge)){
        return true;
    }
    else{
        return false;
    }
}

void reset_bool(bool bool_x[]){
  for(int i = 0; i < 3; i++){
    bool_x[i] = 0;
  }
}