#include <Arduino.h>
#include <LibRobus.h>
#include "utils/ema.h"

// mettre dans tous les fichiers.h
#include <Arduino.h>

float average_read(int pin_analog, int sample_size, int sample_time) 
{
    float mean = 0;

    for (int i = 0; i < sample_size; i++)
    {
        int reading = analogRead(pin_analog);
        mean += reading / sample_size;
        delay(int(sample_time / 2));
    }
    return mean;
}

float difference(int pin_ambiant, int pin_5khz, int interval, int sample_size, int sample_time)
{
    float average_reading_ambiant = average_read(pin_ambiant, sample_size, sample_time);
    float average_reading_5khz = average_read(pin_5khz, sample_size, sample_time);

    float difference = average_reading_5khz - average_reading_ambiant;

    delay(interval);

    return difference;
    
/*     Serial.println
    (
    String(">") +
    "avgA:" + String(average_reading_ambiant) + "," +
    "avg5:" + String(average_reading_5khz) + "," +
    "iA:" + String(integral_ambiant) + "," +
    "i5:" + String(integral_5khz) + "," + 
    "id:" + String(integral_difference)
    );  */

}


bool is_sifflet(/* EmaContext f_ambiant, EmaContext f_5khz, */int pin_ambiant, int pin_5khz, int patience, float thr, int interval) 
{
    float sum = 0.0;
    for (int i = 0; i < patience; i++) {
        sum += difference(pin_ambiant, pin_5khz, interval, 5, 10);
        //sum += difference_ema(f_ambiant, f_5khz);
        if (sum > thr / patience * i) {
            return true;
        }
    }
    //Serial.println("false");
    return false;
}

float difference_ema(EmaContext a, EmaContext b) 
{
    return b.current - a.current;
}

int pin_ambiant = A0;
int pin_5khz = A3;
bool started_robot = false;
EmaContext f_ambiant = make_EMA(10, .1);
EmaContext f_5khz = make_EMA(10, .1);

void setup()
{
    Serial.begin(115200);
    pinMode(pin_ambiant, INPUT);
    pinMode(pin_5khz, INPUT);
}



void loop()
{
    if (!started_robot && is_sifflet(pin_ambiant, pin_5khz, 5, 0, 100)) 
    { // Vérifie sur un temps de (patience * interval + sample_time * sample_size) ms si le sifflet à été entendu
        started_robot = true;
        Serial.println("J'ai entendu le sifflet");
    } else if (!started_robot) {
        Serial.println("Je n'ai pas entendu le sifflet");
    }

    if (started_robot) {
        // Tous les comportements du robot après avoir entendu le sifflet devront être dans ce if statement
        Serial.println("Let's go ça part");
    }
}
