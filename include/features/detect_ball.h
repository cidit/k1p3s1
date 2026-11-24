#pragma once
#include <Arduino.h>
#include "Adafruit_TCS34725.h"

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

namespace detect_ball
{
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
        uint16_t clear, red, green, blue;
        tcs.setInterrupt(false); // turn on LED

        tcs.getRawData(&red, &green, &blue, &clear);

        tcs.setInterrupt(true); // turn off LED

        Serial.print("R:\t");
        Serial.print(red);
        Serial.print("\tG:\t");
        Serial.print(green);
        Serial.print("\tB:\t");
        Serial.println(blue);
        Serial.print("\tC:\t");
        Serial.println(clear);
    }
}
