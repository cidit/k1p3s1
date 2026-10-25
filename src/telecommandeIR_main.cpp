// 1. Crucial fix: Change .h to .hpp to define IrSender
#include <Arduino.h>
//#include <LibRobus.h>
#include <IRremote.hpp> 

const int pin_IR = 9; 
const int start_button = 12;
const int wake_button = 13;

const uint16_t start_address = 0xC001;
const uint16_t wake_address = 0xC002;

const uint16_t target_address = 0x012B;

void setup() {
  Serial.begin(115200);
  
  pinMode(start_button, INPUT_PULLUP);
  pinMode(wake_button, INPUT_PULLUP);
  
  IrSender.begin(pin_IR); 
}

void loop() {
  uint8_t repeats = 0;
  if (!digitalRead(start_button))
  {
    IrSender.sendNEC(target_address, start_address, repeats);
    delay(2000);
  }
  if (!digitalRead(wake_button))
  {
    IrSender.sendNEC(target_address, wake_address, repeats);
    delay(2000);
  }
}
