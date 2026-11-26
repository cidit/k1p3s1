#pragma once
#include <Arduino.h>
#include <features/robot_state.h>
#include <Bouton.h>

namespace button_code
{

    enum State
    {
        READING,
        DONE,
    };

    const uint8_t _BUTTON_PIN = 13;
    const uint32_t _TIME_INPUT = 1000;
    const uint32_t _TIME_DEBOUNCE = 30;
    State _state = DONE;

    bool _is_first_input = true;
    bool _is_still_pressed = false;
    uint32_t _time_at_first_input;
    uint32_t _time_at_first_press;
    uint32_t _time_at_input;
    uint8_t code = 0;

    bool button_clicked()
    {
        return digitalRead(_BUTTON_PIN) == LOW;
    }

    void add_press() {
        code++;
    }
/*     uint8_t code_reading(uint8_t pin, uint32_t start)
    {
        bool is_first_press = true;
        uint32_t code = 0;
        uint32_t time_at_first_press;

        if (_state == DONE)
        {
            code = 1;
            _state = READING;
            Serial.println("Reading : " + String(_state));
        }

        if (button_clicked() and is_first_press)
        {
            time_at_first_press = millis();
            is_first_press = false;
        }

        if (!is_first_press and millis() - time_at_first_press)
    } */

    void begin()
    {
        pinMode(_BUTTON_PIN, INPUT_PULLUP);
        Serial.println("Ready to input button codes : " + String(_state));
    }

    void update()
    {
        if (button_clicked() and _is_first_input)
        {
            _time_at_first_input = millis();
            _is_first_input = false;
            _is_still_pressed = true;
        }

        if (!_is_first_input and millis() - _time_at_first_input < _TIME_DEBOUNCE)
        {
            return;
        }
        else
        {
            if (button_clicked())
            {
                
                if (_state == DONE) {
                    _state = READING;
                    _time_at_first_press = millis();
                    code = 1; 
                } else {
                    if (!_is_still_pressed) {add_press();}
                }
                _is_still_pressed = false;
                
            } else {
            }
            _is_first_input = true;
        }
        
        if (_state == READING and millis() - _time_at_first_press > _TIME_INPUT) {
            _state = DONE;
            Serial.println(String(code));
        }


        /*                 uint32_t _time_at_first_input = millis();
                        code = button_input(_BUTTON_PIN, start);
                        Serial.println("Code received : " + String(code));} */
    }

}
// async : make sure that only one button input can be read per update, as to remove the use of delays