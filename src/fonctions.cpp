#include "fonctions.h"
#include <LibRobus.h>


void forwardInMeters(float meters) {
  float offset = 0.0;
  float target = ((meters+offset) / circumference) * ticks_per_turn;

  float kp = 0.00006;
  float ki = 0.00000001;
  float kp_correction = 0.005; 

  float accumulated_error0 = 0.0;
  float accumulated_error1 = 0.0;
  

  ENCODER_Reset(0);
  ENCODER_Reset(1);

  while (abs(target - ENCODER_Read(0)) > 10 and abs(target - ENCODER_Read(1)) > 10) {
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
  float offset = 1.8*(degrees/abs(degrees));
  float target = ((degrees+offset)/360.0) * circle * (ticks_per_turn/circumference);
  float kp = 0.000001;
  float ki = 0.0000000959;
  float kd = 0.02;
  float accumulated_error0 = 0.0;
  float accumulated_error1 = 0.0;
  ENCODER_Reset(0);
  ENCODER_Reset(1);
  float previous_error0 = target - ENCODER_Read(0);
  float previous_error1 = target + ENCODER_Read(1);
  
  
  while (abs(target - ENCODER_Read(0)) > 5 && abs(target + ENCODER_Read(1)) > 5){

  float error0 = target - ENCODER_Read(0);
  float error1 = target + ENCODER_Read(1);

  float derivative0 = error0 - previous_error0;
  float derivative1 = error1 - previous_error1;
  
  float speed0 = (error0 * kp) + (accumulated_error0 * ki) + (derivative0 * kd);
  float speed1 = -((error1 * kp) + (accumulated_error1 * ki) + (derivative1 * kd));
  //float speed0 = (error0 * kp) + (accumulated_error0 * ki);
  //float speed1 = -((error1 * kp) + (accumulated_error1 * ki));
  //float speed0 = error0 * kp;
  //float speed1 = -error1 * kp;

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
void finalDance(float degrees) {
  float target = (degrees / 360.0) * circle * (ticks_per_turn / circumference);
  
  float kp = 0.05;
  float kd = 0.01;
  float previous_error0 = target - ENCODER_Read(0);
  float previous_error1 = target + ENCODER_Read(1);

  ENCODER_Reset(0);
  ENCODER_Reset(1);
  while (abs(target - ENCODER_Read(0)) > 10 || abs(target + ENCODER_Read(1)) > 10) {

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
void turn_right(){
  rotationPID(90);
}
void turn_left(){
  rotationPID(-90);
}
void forwardInMetersTrapeze(float meters) {
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
    if(speed < abs(target_speed) and millis() - start >= 50 and abs((ENCODER_Read(0) + ENCODER_Read(1))/2) < abs(target) * 0.3){
      //Accélère
    speed +=0.02 + it * 0.005 * signe;
    start = millis();
    it ++;
    }
    else if(abs((ENCODER_Read(0) + ENCODER_Read(1))/2) > abs(target) * 0.6 and millis() - start >= 50 and abs(speed) > 0.2){
      speed -=0.05 * signe;
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