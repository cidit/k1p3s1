#pragma once
#include <Arduino.h>
#include "Adafruit_TCS34725.h"

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

namespace detect_ball
{
    int _time_max = 5000; //Temps max entre les balles
    int _threshold = 500; //Seuil max de C pour déterminer si une balle est détectée
    int _time_last_ball = 0 ;

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
            Serial.println("Ball detected");

        }

        // Si aucune balle n'est détectée, vérifie si le temps maximal
        // est écoulé.
        else
        {
            if ((millis() - _time_last_ball) > _time_max)
            {
                // Partie se termine si le temps est écoulé
                Serial.println("Partie terminée");
                //Serial.println(_time_last_ball); // Print pour débuggage
                //Serial.println(millis()); // Print pour débuggage
            }
        }
    }
}
