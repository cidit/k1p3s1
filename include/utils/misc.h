#pragma once
#include <Arduino.h>

using pin_t = uint8_t;
using timestamp_t = uint32_t;

// FIXME: should add a way to specify the timestamp as per Teleplot's documentation.

/**
 * Using this function assumes the use of some sort of serial graphing 
 * tool like Teleplot. It can be found in the VSCode extenstions marketplace.
 */
#define GRAPH(item) Serial.println(">" + String(#item) + ':' + String(item) + "|np");

#define PLOT(name, x, y) Serial.println(">" + String(name) + ':' + String(x) + ':' + String(y) + "|xy");