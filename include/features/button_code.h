#pragma once
#include <Arduino.h>
#include <features/robot_state.h>
#include <Bouton.h>
#include <features/turret_aiming.h>

namespace button_code
{

    enum State
    {
        READING,
        DONE,
    };

    const uint32_t _TIME_FOR_INPUT = 1000;
    const uint32_t _TIME_FOR_DEBOUNCE = 30;

    uint8_t _BUTTON_PIN;

    State _state = DONE;

    uint8_t code = 0;

    bool _is_initializing_input = true;
    uint32_t _time_at_this_press = 0;

    bool _is_debouncing = false;
    uint32_t _time_at_debouncing = 0;

    bool _is_rising_edge = true;

    bool button_clicked()
    {
        return digitalRead(_BUTTON_PIN) == LOW;
    }

    void begin(uint8_t pin)
    {
        _BUTTON_PIN = pin;
        pinMode(_BUTTON_PIN, INPUT_PULLUP);
        Serial.println("Ready to input button codes : " + String(_state));
    }

    bool started_debounce = false, wait_for_falling_edge = false;

    void update()
    {
        if (button_clicked())
        {
            if (!_is_debouncing)
            {
                _is_debouncing = true;
                _time_at_debouncing = millis();
            }

            if (millis() - _time_at_debouncing < _TIME_FOR_DEBOUNCE)
            {
                //Serial.println("Still debouncing, click doesn't count");
                return;
            }
            else
            {
                _is_debouncing = false;
                //Serial.println("Debouncing has stopped");
            }
            
            if (_is_rising_edge)
            {
                if (_is_initializing_input)
                {
                    _is_initializing_input = false;
                    _state = READING;
                    PRINT(_state);
                    code = 1;
                    Serial.println("First press entered");
                }
                else
                {
                    code += 1;
                    Serial.println("Further press received");
                }
                _is_rising_edge = false;
                _time_at_this_press = millis();
            }
        }
        else if (!_is_rising_edge and millis() - _time_at_debouncing < _TIME_FOR_DEBOUNCE)
        {
            _is_rising_edge = true;
        }

        if (!_is_initializing_input and millis() - _time_at_this_press > _TIME_FOR_INPUT)
        {
            Serial.println("Code received : " + String(code));
            _is_initializing_input = true;
            _state = DONE;
            PRINT(_state);
        }
    }
}
