#ifndef FONCTIONS_H
#define FONCTIONS_H

#include <Arduino.h>
#include <LibRobus.h>
#include <hardware.h>

const float circumference = 0.24; // m
const float dist_roue = (WHEEL_DISTANCE_OUTER - WHEEL_THICKNESS) / 1000.0; // m
const float circle = dist_roue*3.1416; // m
const float ticks_per_turn = 3200.0; //ticks



void forwardInMeters(float meters, uint8_t back = 1);

float distance(int pin);

void rotationPID(float degrees);
void finalDance(float degrees);
void turn_right();
void turn_left();
void turn_more_right();
void turn_more_left();

bool check_open();
void forwardInMetersTrapeze(float meters, int8_t back);

void forwardcorrection(float baseSpeed = 0.5, uint8_t back = 1);

#endif // FONCTIONS_H
