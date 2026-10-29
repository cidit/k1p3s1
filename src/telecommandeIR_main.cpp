// 1. Crucial fix: Change .h to .hpp to define IrSender
#include <Arduino.h>
//#include <LibRobus.h>
#include <IRremote.hpp> 

const int pin_IR = 9; 
const int start_button = 12;
const int wake_button = 13;

const uint16_t start_command = 0xC001;
const uint16_t wake_command = 0xC002;

const uint16_t target_address = 0x012B;

void setup() {
  Serial.begin(115200);
  
  pinMode(start_button, INPUT_PULLUP);
  pinMode(wake_button, INPUT_PULLUP);
  
  IrSender.begin(pin_IR);
  pinMode(pin_IR,OUTPUT);
}

void loop() {
uint8_t repeats = 0;
  if (!digitalRead(start_button))
  
  {digitalWrite(pin_IR, HIGH);} 
  else {digitalWrite(pin_IR,LOW);}
/*     IrSender.sendNEC(target_address, start_command, repeats);
    delay(2000);
  }
  if (digitalRead(wake_button))
  {
    IrSender.sendNEC(target_address, wake_command, repeats);
    delay(2000); 
  } */

}
