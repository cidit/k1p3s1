#pragma once
#include <Arduino.h>

using pin_t = uint8_t;
using timestamp_t = uint32_t;

#define GRAPH(item) Serial.println(">" + String(#item) + ':' + String(item));
