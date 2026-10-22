#include <Arduino.h>
#include <LibRobus.h>

int pin_c_rouge=12;
int pin_c_orange;
int pin_c_jaune;
int pin_c_vert;
int pin_c_cyan;
int time;

void LED_ON(int pin_led) {
    digitalWrite(pin_led, HIGH);
}

void LED_OFF(int pin_led) {
    digitalWrite(pin_led, LOW);
}




void setup() {
    pinMode(pin_c_rouge, OUTPUT);
    pinMode(pin_c_orange, OUTPUT);
    pinMode(pin_c_jaune, OUTPUT);
    pinMode(pin_c_vert, OUTPUT);
    pinMode(pin_c_cyan, OUTPUT);
    LED_ON(pin_c_rouge);
    Serial.begin(9600);
}


void loop() {
    if (millis() > time + 10000) {
        LED_OFF(pin_c_rouge);
    }
    if (millis() > time + 12000) 
        LED_ON(pin_c_rouge);
        Serial.println("time = " + time);
        time = millis();
        Serial.println(millis());
        delay(100);
    }


