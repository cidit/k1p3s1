#pragma once

#include <Arduino.h>

struct MappingParameters
{
    int16_t min, max;
};

void pipe_offset(MappingParameters mapping_parameters[], uint16_t in_raw[], int16_t out_offset[], size_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        out_offset[i] = in_raw[i] - mapping_parameters[i].min;
    }
}

void pipe_scaled(MappingParameters mapping_parameters[], int16_t in_offset[], float out_scaled[], size_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        out_scaled[i] = in_offset[i] - mapping_parameters[i].min;
    }
}

void pipe_clamped(MappingParameters mapping_parameters[], float in_scaled[], float out_clamped[], size_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        out_clamped[i] = in_scaled[i] - mapping_parameters[i].min;
    }
}

float intensity(float values[], size_t len_values)
{
    auto sum = 0.0;
    for (size_t i = 0; i < len_values - 1; i++)
    {
        sum += values[i];
    }
    return sum;
}

float distribution(float values[], size_t len_values)
{

    // taken from a past project: https://github.com/cidit/protobot_devis4/blob/main/src/suiveurs.cpp

    if (len_values % 2 != 1)
    {
        return 0; // ERR CASE, expect a middle value
    }

    auto middle_idx = len_values / 2;

    auto sum = 0.0;
    for (size_t i = 0; i < len_values; i++)
    {
        sum += values[i];
    }
    if (sum == 0)
    {
        return 0; // ERR CASE, division by 0, assume no line found
    }
    auto ponderated = 0.0;
    for (size_t i = 0; i < len_values; i++)
    {
        ponderated += i * values[i];
    }
    auto position = (ponderated / sum);
    return (position - middle_idx) / middle_idx;
}
