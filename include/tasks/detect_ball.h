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
    void setup() {
    tcs.begin();
    }
    void loop() {
        uint16_t r, g, b, c;

        tcs.getRawData(&r, &g, &b, &c);
        
        if (c > threshold);
        throw_ball
        
        else ;
        millis()

        
    }
