#pragma once
#include <Arduino.h>
#include <features/robot_state.h>

namespace button_code {

    const uint8_t _BUTTON_PIN = 13;
    const uint32_t _TIME_INPUT = 1000;
    const uint32_t _TIME_DEBOUNCE = 30;

    bool _is_first_input = true;
    uint32_t _time_at_first_input;
    uint32_t _time_at_input;
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

    bool button_clicked() {
        return digitalRead(_BUTTON_PIN) == LOW;
    }

    void begin() {
        pinMode(_BUTTON_PIN, INPUT_PULLUP);
        Serial.println("Ready to input button codes");
    }

    void update() {
        if (button_clicked() and _is_first_input) {
            _time_at_first_input = millis();
            _is_first_input = false;
        }

            if (!_is_first_input and millis() - _time_at_first_input < _TIME_DEBOUNCE) {
                return;
            } else {
                if (button_clicked()) {
                    _enter_button_read();
                }
            }
`  
               
/*                 uint32_t _time_at_first_input = millis();
                code = button_input(_BUTTON_PIN, start);
                Serial.println("Code received : " + String(code));} */
        }


    }
}
// async : make sure that only one button input can be read per update, as to remove the use of delays