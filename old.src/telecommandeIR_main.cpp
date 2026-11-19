#include <Arduino.h>
#include <IRremote.hpp>

const int pin_IR = 9; 
const int start_button = 12;
const int wake_button = 13;
const int command_led = 4; 
const int power_led = 10;


uint32_t start_command = 0xB74810EF; // mirrored is 0x(F708)(12ED)
uint32_t wake_command = 0xB74811EE; // mirrored is 0x(7788)(12ED)







void setup() {
  Serial.begin(115200);
  
  pinMode(start_button, INPUT_PULLUP);
  pinMode(wake_button, INPUT_PULLUP);
  pinMode(command_led, OUTPUT);
  pinMode(power_led, OUTPUT);
  digitalWrite(power_led, LOW);
  IrSender.begin(pin_IR, ENABLE_LED_FEEDBACK);
}

void loop() {
  digitalWrite(command_led, HIGH);

  if (!digitalRead(start_button)) {
    delay(20);
    if (!digitalRead(start_button)) {
      IrSender.sendNECMSB(bitreverse32Bit(start_command), 32);
      digitalWrite(command_led, LOW);
      delay(1500);
    }
  }
   else if (!digitalRead(wake_button)) {
    IrSender.sendNECMSB(bitreverse32Bit(wake_command), 32);
    digitalWrite(command_led, LOW);
    delay(1500);
  }
}
