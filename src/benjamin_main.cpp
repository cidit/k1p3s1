#include <Arduino.h>

const uint8_t BUTTON_PIN = 13;
const uint32_t TIME_WINDOW = 1000;
const uint32_t TIME_DEBOUNCE = 30;

uint8_t code_read(uint8_t pin, uint32_t start) {
    uint8_t code = 1;
    bool last = LOW;
    uint32_t last_change = millis();

    while (millis() - start < TIME_WINDOW) {
        bool state = digitalRead(pin);

        if (state != last and millis() - last_change > TIME_DEBOUNCE) {
            last = state;
            last_change = millis();
            if (state == LOW) {code++;}
        }
    }
    return code;
}

void setup() {
    Serial.begin(115200);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    Serial.println("ready");
}

void loop() {
    
    if (digitalRead(BUTTON_PIN) == LOW) {
        delay(TIME_DEBOUNCE);
        uint32_t start = millis();
        uint8_t code = code_read(BUTTON_PIN, start);
        Serial.println("Code received : " + String(code));
    }
}