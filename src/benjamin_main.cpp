#include <Arduino.h>
#include <LibRobus.h>

// crash course c++

// mettre dans tous les fichiers.h
#include <Arduino.h>

void differential_read(int pin_ambiant, int pin_ampli, int interval, int sample_size, int sample_time)
{

    float sum_ambiant = 0;
    float sum_5khz = 0;
    float next_sum_ambiant = 0;
    float next_sum_5khz = 0;

    for (int i = 0; i < sample_size; i++)
    {
        int reading_ambiant = analogRead(pin_ambiant);
        int reading_5khz = analogRead(pin_ampli);
        sum_ambiant += reading_ambiant;
        sum_5khz += reading_5khz;
        delay(sample_time);
    }
    float average_reading_ambiant = sum_ambiant / sample_size;
    float average_reading_5khz = sum_5khz / sample_size;

    delay(interval);

    for (int i = 0; i < sample_size; i++)
    {
        int next_reading_ambiant = analogRead(pin_ambiant);
        int next_reading_5khz = analogRead(pin_ampli);
        next_sum_ambiant += next_reading_ambiant;
        next_sum_5khz += next_reading_5khz;
        delay(sample_time);
    }
    float average_next_reading_ambiant = next_sum_ambiant / sample_size;
    float average_next_reading_5khz = next_sum_5khz / sample_size;

    float diff_ambiant = float(average_next_reading_ambiant - average_reading_ambiant) / interval;
    float diff_5khz = float(average_next_reading_5khz - average_reading_5khz) / interval;

    Serial.println(String(">") +
                   "avgA:" + String(average_reading_ambiant) + "," +
                   "avgNA:" + String(average_next_reading_ambiant) + "," +
                   "avg5:" + String(average_reading_5khz) + "," +
                   "avgN5:" + String(average_next_reading_5khz) + "," +
                   "dA:" + String(diff_ambiant) + "," +
                   "d5:" + String(diff_5khz));
}

void integral_read(int pin_ambiant, int pin_ampli, int interval, int sample_size, int sample_time)
{

    float sum_ambiant = 0;
    float sum_5khz = 0;
    float next_sum_ambiant = 0;
    float next_sum_5khz = 0;

    for (int i = 0; i < sample_size; i++)
    {
        int reading_ambiant = analogRead(pin_ambiant);
        int reading_5khz = analogRead(pin_ampli);
        sum_ambiant += reading_ambiant;
        sum_5khz += reading_5khz;
        delay(sample_time);
    }
    float average_reading_ambiant = sum_ambiant / sample_size;
    float average_reading_5khz = sum_5khz / sample_size;

    delay(interval);

    for (int i = 0; i < sample_size; i++)
    {
        int next_reading_ambiant = analogRead(pin_ambiant);
        int next_reading_5khz = analogRead(pin_ampli);
        next_sum_ambiant += next_reading_ambiant;
        next_sum_5khz += next_reading_5khz;
        delay(sample_time);
    }
    float average_next_reading_ambiant = next_sum_ambiant / sample_size;
    float average_next_reading_5khz = next_sum_5khz / sample_size;

    float integral_ambiant = float(average_next_reading_ambiant - average_reading_ambiant) * interval;
    float integral_5khz = float(average_next_reading_5khz - average_reading_5khz) * interval;

    Serial.println(String(">") +
                   "avgA:" + String(average_reading_ambiant) + "," +
                   "avgNA:" + String(average_next_reading_ambiant) + "," +
                   "avg5:" + String(average_reading_5khz) + "," +
                   "avgN5:" + String(average_next_reading_5khz) + "," +
                   "iA:" + String(integral_ambiant) + "," +
                   "i5:" + String(integral_5khz));
}

int pin_ambiant = A0;
int pin_ampli = A3;

void setup()
{
    Serial.begin(115200);
    pinMode(pin_ambiant, INPUT);
    pinMode(pin_ampli, INPUT);
}

void loop()
{
    // int ambiant_read;
    // int ampli_read;
    // ambiant_read = analogRead(pin_ambiant);
    // ampli_read = analogRead(pin_ampli);
    // Serial.print(ambiant_read);
    // Serial.print(" ");
    // Serial.println(ampli_read);
    differential_read(pin_ambiant, pin_ampli, 500, 10, 5);
}
