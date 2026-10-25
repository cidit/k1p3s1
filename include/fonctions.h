#pragma once
#ifndef FONCTIONS_H
#define FONCTIONS_H

#include <Arduino.h>
#include <LibRobus.h>
#include <hardware.h>

const int pin_vert = 48;
const int pin_rouge = 49;
const int pin_ambiant = A0;
const int pin_5khz = A3;
const float circumference = 0.24; // m
const float dist_roue = (WHEEL_DISTANCE_OUTER - WHEEL_THICKNESS) / 1000.0; // m
const float circle = dist_roue*3.1416; // m
const float ticks_per_turn = 3200.0; //ticks


void forwardInMeters(float meters);

void rotationPID(float degrees);
void finalDance(float degrees);
void turn_right();
void turn_left();
void turn_more_right();
void turn_more_left();

bool check_open();
void forwardInMetersTrapeze(float meters);

float difference_read(int pin_ambiant, int pin_5khz, int interval, int sample_size, int sample_time);
bool is_sifflet(int pin_ambiant, int pin_5khz, int patience, float integral_thr);

enum {rouge, orange, jaune, vert, bleu, nb_leds};

struct ColoredLED
{
    int pin;
    unsigned long time_set;
    bool is_on;

    ColoredLED(int led_pin);

    void begin();
    void on();
    void off();
    bool is_time();
};

extern ColoredLED colored_leds[nb_leds];

void start_colored_leds(ColoredLED colored_leds[]);
void check_colored_leds(ColoredLED colored_leds[]);

float average_read(int pin_analog, int sample_size, int sample_time);
float difference(int pin_ambiant, int pin_5khz, int interval, int sample_size, int sample_time);
bool is_sifflet(/* EmaContext f_ambiant, EmaContext f_5khz, */int pin_ambiant, int pin_5khz, int patience, float thr, int interval); 
void reset_bool(bool bool_x[]);

#endif // FONCTIONS_H
