#include <Arduino.h>
#include <IRremote.hpp> 

const int pin_IR = 9; 
const int start_button = 12;
const int wake_button = 13;
const int command_led = 4; 
const int power_led = 10;

const uint16_t start_command = 0x80;
const uint16_t wake_command = 0x81;

const uint16_t target_address = 0xAA;
const int repeats = 0;

void setup() {
  Serial.begin(115200);
  
  pinMode(start_button, INPUT_PULLUP);
  pinMode(wake_button, INPUT_PULLUP);
  pinMode(command_led, OUTPUT);
  pinMode(power_led, OUTPUT);

  IrSender.begin(pin_IR, ENABLE_LED_FEEDBACK);
  digitalWrite(power_led, LOW);
}

void loop() {
  digitalWrite(command_led, HIGH);

  if (!digitalRead(start_button)) {
    IrSender.sendNEC(target_address, start_command, repeats); 
    digitalWrite(command_led, LOW);
    delay(1500);
  }
   else if (!digitalRead(wake_button)) {
     IrSender.sendNEC(target_address, wake_command, repeats);
    digitalWrite(command_led, LOW);
     delay(1500);
  }
}
