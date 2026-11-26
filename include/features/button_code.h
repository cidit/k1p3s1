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
        // Serial.println("Ready to input button codes : " + String(_state));
        Serial.println("FEATURE READY : BUTTON_CODE");
    }

    bool started_debounce = false, wait_for_falling_edge = false;

    void update()
    {
        if (button_clicked()) // Au début d'update(), le programme lit si le bouton est appuyé
        {
            if (!_is_debouncing) // Le programme lit si le debounce a été effectué
            {
                _is_debouncing = true;
                _time_at_debouncing = millis();
            }

            if (millis() - _time_at_debouncing < _TIME_FOR_DEBOUNCE) // Le programme lit si le debouncing est terminé
            {
                // Serial.println("Still debouncing, click doesn't count");
                return;
            }
            else // À partir d'ici, on considère le bouton comme appuyé par un humain
            {
                _is_debouncing = false;
                // Serial.println("Debouncing has stopped");
            }

            if (_is_rising_edge) // On regarde si le programme en est à sa première fois d'effectuer le code plus bas pour un seul appui (on ne veut pas qu'il considère le même appui comme plusieurs différents)
            {
                if (_is_initializing_input)
                {
                    _is_initializing_input = false;
                    robot_state::game_sleep();
                    _state = READING;
                    // PRINT(_state);
                    code = 1;
                    // Serial.println("First press entered");
                }
                else
                {
                    code += 1;
                    // Serial.println("Further press received");
                }
                _is_rising_edge = false; // On dit explicitement que le bouton n'en est plus à sa première exécution du même appui (bloque la condition plus haut pour un même appui de bouton)
                _time_at_this_press = millis();
            }
        }
        else if (!_is_rising_edge and _is_debouncing) // Si on lit le bouton à LOW et que nous ne sommes pas en période de debouncing, on conclut que l'humain a cessé d'appuyer sur le bouton
        {
            _is_rising_edge = true;
        }

        if (!_is_initializing_input and millis() - _time_at_this_press > _TIME_FOR_INPUT) // Si ça fait un certain temps que l'humain n'a pas appuyé sur le bouton, on conclut que l'humain a finit d'entrer son code
        {
            Serial.println("Code received : " + String(code));
            _is_initializing_input = true;
            _is_debouncing = false;
            _state = DONE;
            // PRINT(_state);
        }
    }
}
