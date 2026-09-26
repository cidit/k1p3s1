#include <Arduino.h>
#include <librobus.h>

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
const float kp = 0.01;

void brake(){
  for(int i = speed; i > 0; i--){
    MOTOR_SetSpeed(0, i);
    MOTOR_SetSpeed(1, i);
    delay(100);
  }
}

void rotationPID(float degrees){
  float target = (degrees/360.0) * circle * (ticks_per_turn/circumference);
  float kp = 0.00008;
  float ki = 0.00000705;
  float accumulated_error0 = 0.0;
  float accumulated_error1 = 0.0;
  Serial.print("target : ");
  Serial.println(target);
  ENCODER_Reset(0);
  ENCODER_Reset(1);
  while (target - ENCODER_Read(0) > 1 && target + ENCODER_Read(1) > 1){
  Serial.print("ENCODER 0 : ");
  Serial.println(target - ENCODER_Read(0));
  Serial.print("ENCODER 1 : ");
  Serial.println(target - ENCODER_Read(1));
  float error0 = target - ENCODER_Read(0);
  float error1 = -(target + ENCODER_Read(1));
  
  float speed0 = (error0 * kp) + (accumulated_error0 * ki);
  float speed1 = (error1 * kp) + (accumulated_error1 * ki);
  //float speed0 = (error0 * kp);
  //float speed1 = (error1 * kp);
    
  Serial.print("speed0 : ");
  Serial.println(speed0);
  Serial.print("speed1 : ");
  Serial.println(speed1);
  MOTOR_SetSpeed(0, speed0);
  MOTOR_SetSpeed(1, speed1);
  accumulated_error0 += error0;
  accumulated_error1 += error1;
  }
  MOTOR_SetSpeed(0, 0);
  MOTOR_SetSpeed(1, 0);
}

void rotation(int degrees){ // 90
  float proportion = float(degrees)/360.0;
  float target = circle * proportion / (circumference/ticks_per_turn);
  
  while (target - ENCODER_Read(0) > 10 && target - ENCODER_Read(1) > 10){
    float speed = target - ENCODER_Read(0);
    MOTOR_SetSpeed(0, speed);
    MOTOR_SetSpeed(1, -speed);
  }
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
  rotationPID(270);
  delay(2000);
  
  
}
