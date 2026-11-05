#include <Arduino.h>
#include "fonctions.h"
#include <LibRobus.h>
#include <MegaServo/MegaServo.h>

void setup() {
  Serial.begin(9600);
  BoardInit();
  
  servo_up.attach(2);
  servo_pince.attach(3);
  quille();
}



void loop() {

}


