#pragma once

#include <Arduino.h>
#include <LibRobus.h>
#include <hardware.h>
#include <MegaServo/MegaServo.h>

const float circumference = 0.24; // m
const float dist_roue = (WHEEL_DISTANCE_OUTER - WHEEL_THICKNESS) / 1000.0; // m
const float circle = dist_roue*3.1416; // m
const float ticks_per_turn = 3200.0; //ticks



extern MegaServo servo_up;
extern MegaServo servo_pince;


void raph_rotationPID(float degrees);

void raph_forwardInMetersTrapeze(float meters, int8_t back);

void pincer();

void lacher();

void verre();

void quille();

float ticks_to_deg(int ticks);