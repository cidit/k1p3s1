#pragma once
#include <Arduino.h>

namespace button_code {

    const uint8_t _BUTTON_PIN = 13;
    const uint32_t _TIME_INPUT = 1000;
    const uint32_t _TIME_DEBOUNCE = 30;
    uint8_t code;

    uint8_t button_input(uint8_t pin, uint32_t start) {
        uint8_t code = 1;
        bool last_state = LOW;
        uint32_t time_at_change = millis();

        while (millis() - start < _TIME_INPUT) {
            bool state = digitalRead(pin);

            if (state != last_state and millis() - time_at_change > _TIME_DEBOUNCE) {
                last_state = state;
                time_at_change = millis();
                if (state == LOW) {
                    code++;
                }
            }
        }

        return code;
    }

    void begin() {
        pinMode(_BUTTON_PIN, INPUT_PULLUP);
        Serial.println("Ready to input button codes");
    }

    void update() {
        if (digitalRead(_BUTTON_PIN) == LOW) {
            delay(_TIME_DEBOUNCE);
            uint32_t start = millis();
            code = button_input(_BUTTON_PIN, start);
            Serial.println("Code received : " + String(code));
        }
    }
}