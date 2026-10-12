#pragma once
#include <Arduino.h>

using pin_t = uint8_t;
using timestamp_t = uint32_t;

/**
 * Using this function assumes the use of some sort of serial graphing 
 * tool like Teleplot. It can be found in the VSCode extenstions marketplace.
 */
#define GRAPH(item) Serial.println(">" + String(#item) + ':' + String(item));
