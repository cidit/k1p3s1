#include <Arduino.h>
#include "fonctions.h"
#include <LibRobus.h>
#include <utils/ema.h>
#include <utils/misc.h>
#include <MegaServo/MegaServo.h>

void setup() {
  Serial.begin(9600);
  BoardInit();
  
  servo_up.attach(2);
  servo_pince.attach(3);

  ENCODER_Reset(0);
  ENCODER_Reset(1);
  // Setup pince
  servo_pince.write(150);
  servo_up.write(120);
  verre();
}



void loop() {

}


