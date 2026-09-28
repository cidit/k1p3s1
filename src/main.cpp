#include <Arduino.h>
#include <LibRobus.h>

void setup() 
{
  BoardInit();
}


int32_t count_0;
int32_t count_1;
const float dist_roue = 0.18359; 
//const float dist_roue = 0.21;
const float circle = dist_roue*3.1416;
const float ticks_per_turn = 3200.0;

float distance_0;
float distance_1;

float circumference = 0.2393893602;

const float speed = 0.7;


void forwardInMeters(float meters){
  float target = meters/circumference * ticks_per_turn;
  Serial.print("target : ");
  Serial.println(target);
  float kp = 0.001;
  float kp_correction = 0.00001;
  ENCODER_Reset(0);
  ENCODER_Reset(1);
  while (abs(target - ENCODER_Read(0)) > 30 || abs(target - ENCODER_Read(1)) > 30){
    int32_t count_0 = ENCODER_Read(0);
    int32_t count_1 = ENCODER_Read(1);
    float diff = count_0 - count_1;
    float max = max(count_0, count_1);
    diff = diff/max;
    float error0 = target - count_0;
    float error1 = target - count_1;
    float speed0 = error0 * kp;
    float speed1 = error1 * kp;
    if (diff != 0){
      float correction = diff * kp_correction;
      MOTOR_SetSpeed(0, speed0 - correction);
      MOTOR_SetSpeed(1, speed1 + correction);
    }
    else{
      MOTOR_SetSpeed(0, speed0);
      MOTOR_SetSpeed(1, speed1);
    }
  }
}

void rotationPID(float degrees){
  float target = (degrees/360.0) * circle * (ticks_per_turn/circumference);
  float kp = 0.0001;
  float ki = 0.00000705;
  float kd = 0.00000008;
  float accumulated_error0 = 0.0;
  float accumulated_error1 = 0.0;
  ENCODER_Reset(0);
  ENCODER_Reset(1);
  float previous_error0 = target - ENCODER_Read(0);
  float previous_error1 = target + ENCODER_Read(1);
  Serial.print("target : ");
  Serial.println(target);
  
  while (abs(target - ENCODER_Read(0)) > 15 && abs(target + ENCODER_Read(1)) > 15){
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
  //float speed0 = (error0 * kp) + (accumulated_error0 * ki);
  //float speed1 = -((error1 * kp) + (accumulated_error1 * ki));
  //float speed0 = (error0 * kp);
  //float speed1 = -(error1 * kp);
    
  Serial.print("speed0 : ");
  Serial.println(speed0);
  Serial.print("speed1 : ");
  Serial.println(speed1);
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



void loop() 
{
  /*
  uint16_t debut = millis();
  MOTOR_SetSpeed(0, speed);
  MOTOR_SetSpeed(1, speed);

  while (millis()- debut < 3000){
    count_0 = ENCODER_Read(0);
    count_1 = ENCODER_Read(1);
    float diff = (count_0 - count_1);
    double max = max(count_0, count_1);
    Serial.print("max : ");
    Serial.println(max);
    double divid = diff/max;
    Serial.print("diff : ");
    Serial.println(diff);
    Serial.print("divid : ");
    Serial.println(divid);
    double fin = divid*kp;
    Serial.print("fin : ");
    Serial.println(fin);
    if(diff != 0){
      
      MOTOR_SetSpeed(0, speed - divid);
      MOTOR_SetSpeed(1, speed + divid);
    }
    
    Serial.print("count_0 : ");
    Serial.println(count_0);
    Serial.print("count_1 : ");
    Serial.println(count_1);

    Serial.print("distance_0 : ");
    Serial.println(float(count_0)/3200.0 * circumference);
    Serial.print("distance_1 : ");
    Serial.println(float(count_1)/3200.0 * circumference);
  }

  MOTOR_SetSpeed(0, 0);
  MOTOR_SetSpeed(1, 0);

  while(true){delay(100);}*/
  
  rotationPID(180);
  delay(2000);
  
}
