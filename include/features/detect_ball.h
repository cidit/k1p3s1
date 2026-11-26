#pragma once
#include <Arduino.h>
#include "Adafruit_TCS34725.h"

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

namespace detect_ball
{
    int _threshold = 500; // Seuil max de valeur C pour déterminer si une balle est détectée
    uint32_t _time_last_ball = 0;

    void begin()
    {
        Serial.println("Color View Test!");
        if (tcs.begin())
        {
            Serial.println("Found sensor");
        }
        else
        {
            Serial.println("No TCS34725 found ... check your connections");
            while (1)
                ; // halt!
        }
    }

    void update()
    {
        uint16_t _clear, _red, _green, _blue;
        tcs.setInterrupt(false); // turn on LED

        tcs.getRawData(&_red, &_green, &_blue, &_clear);

        tcs.setInterrupt(true); // turn off LED

        // Affichage des valeurs RGBC
        /*
        Serial.print("R:\t");
        Serial.print(_red);
        Serial.print("\tG:\t");
        Serial.print(_green);
        Serial.print("\tB:\t");
        Serial.println(_blue);
        Serial.print("\tC:\t");
        Serial.println(_clear);
        */

        // Si une balle est détectée, le chrono redémarre
        if (_clear > _threshold)
        {
            _time_last_ball = millis();
            // Serial.println("Ball detected");
        }
    }
}
