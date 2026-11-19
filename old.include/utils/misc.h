#pragma once
#include <Arduino.h>

using pin_t = uint8_t;

// FIXME: should add a way to specify the timestamp as per Teleplot's documentation.

/**
 * Using this function assumes the use of some sort of serial graphing
 * tool like Teleplot. It can be found in the VSCode extenstions marketplace.
 */
#define GRAPH(item) Serial.println(">" + String(#item) + ':' + String(item) + "|np");
#define GRAPHT(item) Serial.println(">" + String(#item) + ':' + String(millis()) + ':' + String(item) + "|np");

#define PLOT(name, x, y) Serial.println(">" + String(name) + ':' + String(x) + ':' + String(y) + "|xy");

using milliseconds = uint32_t;
using microseconds = uint32_t;
using seconds = float;

constexpr seconds T_from_Hz(float frequency_hz)
{
    return 1 / frequency_hz;
}

constexpr milliseconds ms_from_s(seconds s)
{
    return s * 1e3;
}

constexpr microseconds us_from_s(seconds s)
{
    return s * 1e6;
}

constexpr seconds s_from_ms(seconds ms)
{
    return ms * 1e-3;
}

constexpr seconds s_from_us(microseconds us) 
{ 
    return us * 1e-6; 
}
