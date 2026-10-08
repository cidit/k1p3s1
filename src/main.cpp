#include <Arduino.h>
#include <LibRobus.h>
#include "fonctions.h"


void setup(){
  pinMode(pin_vert, INPUT);
  pinMode(pin_rouge, INPUT);
  Serial.begin(9600);
  BoardInit();
  delay(2000);
}


void loop(){
}