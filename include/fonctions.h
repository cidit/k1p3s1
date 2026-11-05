#pragma once

#include <Arduino.h>
#include <LibRobus.h>
#include <hardware.h>
#include <utils/ema.h>
#include <utils/misc.h>
#include <MegaServo/MegaServo.h>

const float circumference = 0.24; // m
const float dist_roue = (WHEEL_DISTANCE_OUTER - WHEEL_THICKNESS) / 1000.0; // m
const float circle = dist_roue*3.1416; // m
const float ticks_per_turn = 3200.0; //ticks



MegaServo servo_up; 
MegaServo servo_pince; 


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

void pincer();

void lacher();

void verre();

void quille();
