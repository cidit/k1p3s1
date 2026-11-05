//fonctions.cpp
#include "fonctions.h"
#include <LibRobus.h>
#include <utils/ema.h>
#include <utils/misc.h>

MegaServo servo_up;
MegaServo servo_pince;

void raph_rotationPID(float degrees){
  float offset = 5.0*(degrees/abs(degrees)); // 3b = 5.0 3a = 4.3
  float target = ((degrees+offset)/360.0) * circle * (ticks_per_turn/circumference);
  float kp = 0.002; // 3b = 0.002 3a = 0.003
  float ki = 0.000001;
  float kd = 0.02;
  float accumulated_error0 = 0.0;
  float accumulated_error1 = 0.0;
  ENCODER_Reset(0);
  ENCODER_Reset(1);
  float previous_error0 = target - ENCODER_Read(0);
  float previous_error1 = target + ENCODER_Read(1);
  
  
  while (abs(target - ENCODER_Read(0)) > 100 or abs(target + ENCODER_Read(1)) > 100){

  float error0 = target - ENCODER_Read(0);
  float error1 = target + ENCODER_Read(1);

  float derivative0 = error0 - previous_error0;
  float derivative1 = error1 - previous_error1;
  
  float speed0 = (error0 * kp) + (accumulated_error0 * ki) + (derivative0 * kd);
  float speed1 = -((error1 * kp) + (accumulated_error1 * ki) + (derivative1 * kd));

  if (speed0 > 0.3) speed0 = 0.3;
  else if (speed0 < -0.3) speed0 = -0.3;

  if (speed1 > 0.3) speed1 = 0.3;
  else if (speed1 < -0.3) speed1 = -0.3;

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

void raph_forwardInMetersTrapeze(float meters, int8_t back) {
  float it = 0;
  float target = back * (meters / circumference) * ticks_per_turn;
  float signe = (target != 0) ? (target / abs(target)) : 1.0;
  float kp_correction = 0.005; 
  float target_speed = back * 0.3;
  float speed = 0.2 * signe; // Vitesse initiale orientée selon la direction

  ENCODER_Reset(0);
  ENCODER_Reset(1);
  unsigned long start = millis();

  while (abs(target) > abs((ENCODER_Read(0) + ENCODER_Read(1)) / 2)) {
    // Accélération (Phase 1)
    if (abs(speed) < abs(target_speed) && millis() - start >= 50 && abs((ENCODER_Read(0) + ENCODER_Read(1)) / 2) < abs(target) * 0.3) {
      speed += (0.02 + (it * 0.005)) * signe;
      start = millis();
      it++;
    }
    // Décélération (Phase 3)
    else if (abs((ENCODER_Read(0) + ENCODER_Read(1)) / 2) > abs(target) * 0.6 && millis() - start >= 50 && abs(speed) > 0.2) {
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

void pincer(){
  servo_pince.write(100);
  delay(2000);
  servo_up.write(80);
  delay(2000);
}

void lacher(){
  servo_up.write(120);
  delay(2000);
  servo_pince.write(170);
  delay(2000);
}

void verre(){
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
  while(exit == false) {
    if (millis() - last_sample_time >= 10) {
      last_sample_time = millis();
      EMA_add(moving_avg, ROBUS_ReadIR(1));
      GRAPH(moving_avg.current);
    }

    if (moving_avg.current > treshold) {
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

      if (deg > 180){
        raph_rotationPID(360-deg); 
      }
      else {
        raph_rotationPID(-deg); 
      }

      exit = true;
    }
  } 
}

void quille(){
  int treshold = 200;
  unsigned long last_sample_time = 0;
  EmaContext moving_avg = make_EMA(T_from_Hz(100), 0.1);
  int offset = -15;

  ENCODER_Reset(0);
  ENCODER_Reset(1); 

  MOTOR_SetSpeed(0, 0.2);
  MOTOR_SetSpeed(1, -0.2);

  bool exit = false;
  while(exit == false){
    if (millis() - last_sample_time >= 10) {
      last_sample_time = millis();
      EMA_add(moving_avg, ROBUS_ReadIR(1));
      GRAPH(moving_avg.current);
    }

    if (moving_avg.current > treshold) {
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

      if (deg > 180){
        raph_rotationPID(360-deg); 
      }
      else {
        raph_rotationPID(-deg); 
      }
      exit = true;
    } 
  }
}

float ticks_to_deg(int ticks){
    return ticks * (circumference/ticks_per_turn) * (360 /circle);
}