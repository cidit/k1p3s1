#include <Arduino.h>
#include <librobus.h>

void setup() 
{
  BoardInit();
}


int32_t count_0;
int32_t count_1;

float distance_0;
float distance_1;

float circumference = 0.2394;

const float speed = 0.7;
const float kp = 0.01;

void loop() 
{
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

  while(true){delay(100);}
  
}
/*pseudo pid

diff = enco0 - enco1 // ex : 56 - 50 = 6 donc encore doit aller plus vite

MOTOR_SetSpeed(0, 0.3 - diff);
MOTOR_SetSpeed(1, 0.3 + diff);
*/
void brake(){
  for(int i = speed; i > 0; i--){
    MOTOR_SetSpeed(0, i);
    MOTOR_SetSpeed(1, i);
    delay(100);
  }
}