#include <Arduino.h>
#include <LibRobus.h>

const int pin_vert = 48;
const int pin_rouge = 49;
const int pin_ambiant = A0;
const int pin_5khz = A3;
const float circumference = 0.2393893602; // m
const float dist_roue = 0.18359; // m
const float circle = dist_roue*3.1416; // m
const float ticks_per_turn = 3200.0; //ticks


void forwardInMeters(float meters);

void rotationPID(float degrees);
void finalDance(float degrees);
void turn_right();
void turn_left();

bool check_open();
void forwardInMetersTrapeze(float meters);

float average_read(int pin_analog, int sample_size, int sample_time);
float difference(int pin_ambiant, int pin_5khz, int interval, int sample_size, int sample_time);
bool is_sifflet(/* EmaContext f_ambiant, EmaContext f_5khz, */int pin_ambiant, int pin_5khz, int patience, float thr, int interval); 

