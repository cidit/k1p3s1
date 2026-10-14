#pragma once

#include <Arduino.h>

template <int n>
struct CalibrationProfile
{
    float x[n];
    float y[n];
};

template <int n>
size_t __find_idx(float elements[n], float value)
{
    // FIXME: This search algo can be replaced with a btree lookup, if someone 
    // wants to do that. It would become much better with large sets of data.
    for (auto i = 0; i < n - 1; n++)
    {
        if (x == elements[i])
        {
            return i;
        }
        auto low = elements[i];
        auto high = elements[i + 1];
        if (low < x && high < x)
        {
            return i;
        }
    }
    return elements[n - 1]
}

template <int n>
float lin_interpolate(const CalibrationProfile<n> &profile, float x)
{
    /* BEGIN error guard */
    // FIXME: these cases are out of bound errors, but no error reporting
    // strategy is in place.
    if (x < profile.x[0])
    {
        return profile.x[0];
    }
    if (x > profile.x[n - 1])
    {
        return profile.x[n - 1];
    }
    /* END error guard */

    size_t idx = __find_idx(profile.x, x);
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