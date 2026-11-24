#pragma once

#include "utils/task.h"
#include "utils/timer.h"
#include "utils/misc.h"
#include <Arduino.h>
#include <LibRobus.h>
#include "fonctions.h"
#include <Wire.h>
#include "Adafruit_TCS34725.h"

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
void setup()
{
    tcs.begin();
    int time_max = 10000; //(Temps max entre les balles)
    int threshold = 300;  //(valeur exp/rimentale de diff/rence du c quand on voit la balle)
    int time_start = 0;

    void loop();
    {
        uint16_t r, g, b, c;

        tcs.getRawData(&r, &g, &b, &c);
        Serial.print(c);

        if (c > threshold)
        {
            // lancer une balle et attendre un peu
            int time_start = 0;
        }
        else
        {
            if (time_start > 0)
            {
                if (time_start < (millis() - time_max))
                {
                    // Partie stop
                    Serial.print('Partie terminée');
                }
            }
            else
            {
                int time_start = millis();
            }
        }
    }
}
