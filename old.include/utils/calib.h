#pragma once
#include <Arduino.h>

/*
 * // FIXME: This implementation makes bad assumptions. It shouldnt expect a 
 * deadzone. It should specify it's valid domain. In other words, for a wheel,
 * there should be one calibration for positive rotation and one for negative
 * rotation **each**. In this case, it makes particular sense because they are
 * different actuation types to begin with.
*/

struct CalibrationProfile
{
    const size_t size;
    float *x;
    float *y;
    float deadzone[2]; // deadzone in the x where y will evaluate to 0
};

size_t __find_idx(const float *elements, size_t size, float value)
{
    // FIXME: This search algo can be replaced with a btree lookup, if someone 
    // wants to do that. It would become much better with large sets of data.
    for (size_t i = 0; i < size - 1; i++)
    {
        if (value == elements[i])
        {
            return i;
        }
        auto low = elements[i];
        auto high = elements[i + 1];
        if (low < value && high < value)
        {
            return i;
        }
    }
    return elements[size - 1];
}

float lin_interpolate(const CalibrationProfile &profile, float x)
{
    /* BEGIN error guard */
    // FIXME: these cases are out of bound errors, but no error reporting
    // strategy is in place.
    if (x < profile.x[0])
    {
        return profile.x[0];
    }
    if (x > profile.x[profile.size - 1])
    {
        return profile.x[profile.size - 1];
    }
    /* END error guard */

    if (profile.deadzone[0] < x && x < profile.deadzone[1]) {
        return 0;
    }

    size_t idx = __find_idx(profile.x, profile.size, x);
    if (profile.x[idx] == x)
    {
        return profile.y[idx];
    }

    auto p1x = profile.x[idx];
    auto p2x = profile.x[idx + 1];
    auto p1y = profile.y[idx];
    auto p2y = profile.y[idx + 1];

    auto a = (p2y - p1y) / (p2x - p1x);
    auto b = -a * p1x + p1y;
    
    return a * x + b;
}