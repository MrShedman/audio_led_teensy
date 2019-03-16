#pragma once

#include <Arduino.h>

struct rgb_t
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

struct hsv_t
{
    float h;
    float s;
    float v;
};

// h in degrees, s and v between 0 and 1
rgb_t hsv2rgb(float h, float s, float v);

// h in degrees, s and v between 0 and 1
rgb_t hsv2rgb(hsv_t hsv);