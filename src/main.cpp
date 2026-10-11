#include <Arduino.h>

const int pin_ambiant = A0;
const int pin_ampli = A3;

void setup() {
    Serial.begin(9600);

}

void loop() {
    int ambiant_read;
    int ampli_read;
    ambiant_read = analogRead(pin_ambiant);
    ampli_read = analogRead(pin_ampli);
    Serial.print(ambiant_read);
    Serial.print(" ");
    Serial.println(ampli_read);

}
