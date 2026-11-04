#include <Arduino.h>
#include "fonctions.h"
#include <LibRobus.h>
#include <utils/ema.h>
#include <utils/misc.h>

EmaContext moving_avg = make_EMA(T_from_Hz(100), 0.1);

void setup() {
  Serial.begin(9600);
  BoardInit();

  ENCODER_Reset(0);
  ENCODER_Reset(1);
  analogReference(INTERNAL2V56);
}

const int pin = A0;
int count = 0;

int treshold = 400;

void loop() {
  MOTOR_SetSpeed(0, 0.3);
  MOTOR_SetSpeed(1, -0.3);


  if (millis() % 10 == 0) {
    EMA_add(moving_avg, analogRead(pin));
    GRAPH(moving_avg.current)
  }

  if (moving_avg.current > treshold){
    MOTOR_SetSpeed(0, 0);
    MOTOR_SetSpeed(1, 0);
    uint16_t tick_turn = (abs(ENCODER_Read(0)) + abs(ENCODER_Read(1)))/2;
    Serial.println("Dedans ! ");
    delay(500);
    forwardInMetersTrapeze(0.4, -1);
    delay(500); 
    moving_avg.current = 0;
    forwardInMetersTrapeze(0.4, 1);

    float degrees = tick_turn / (circle * (ticks_per_turn/circumference) *360);
    Serial.println(degrees);
    rotationPID(-degrees); 
  }  
  delay(10000);
}

/* 
  MOTOR_SetSpeed(0, 0.3);
  MOTOR_SetSpeed(1, -0.3);


  if (millis() % 10 == 0) {
    EMA_add(moving_avg, analogRead(pin));
    GRAPH(moving_avg.current)
  }

  float dist = analogRead(pin);

  Serial.println(dist);
  delay(10);
  if (dist > treshold){
    count ++;
  }
  else{
    count = 0;
  }

  if (count > 4){
    MOTOR_SetSpeed(0, 0);
    MOTOR_SetSpeed(1, 0);
    Serial.println("Debut moyenne ");

    delay(1000);

    double timer = millis();
    int nb_echantillons = 0;
    uint16_t sum_echantillons = 0;

    while (millis() - timer < 2000){
      Serial.println(analogRead(pin));
      
      nb_echantillons++;
      sum_echantillons += analogRead(pin);
      delay(40);
    }
    float moyenne =  sum_echantillons/nb_echantillons;
    Serial.print("moyenne : ");
    Serial.println(moyenne);
    delay(1000);

    if (moyenne > treshold){
      Serial.println("Avance");
      uint16_t tick_turn = (abs(ENCODER_Read(0)) + abs(ENCODER_Read(1)))/2;

      dist = 0.4;
      forwardInMetersTrapeze(dist, -1);
      delay(1000); 
      count = 0;
    }
    else{
      Serial.println("Debut");
      return;
    }
  }



/* 


void pincer(){
  servo_pince.write(100);
  delay(2000);
  servo_up.write(100);
  delay(2000);
}

void lacher(){
  servo_up.write(120);
  delay(2000);
  servo_pince.write(170);
  delay(2000);
}

void retour(){
  degrees = tick_turn / (circle * (ticks_per_turn/circumference) *360;
  Serial.println(degrees);
  forwardInMetersTrapeze(dist, 1);
  rotationPID(-degrees);
}

/* 
#include <Arduino.h>
#include <Servo.h>

Servo servo_up; // 120-80
Servo servo_pince; // 170 - 110

void setup() {
  Serial.begin(9600);
  //BoardInit();
  servo_up.attach(2);
  servo_pince.attach(3);
}

void loop() {
  servo_up.write(120);
  delay(2000);
  servo_pince.write(170);
  delay(2000);
  servo_pince.write(100);
  delay(2000);
  servo_up.write(100);
  delay(2000);
}
 */

