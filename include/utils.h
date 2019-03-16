#pragma once

#include <Arduino.h>

#include "pins.h"

uint32_t loop_start_time = 0;
const float loop_rate = 60.0;
const uint32_t loop_time_us = 1e6 / loop_rate;

void spin_once()
{
    while (micros() - loop_start_time < loop_time_us);

    loop_start_time = micros();
}

void init_fan()
{   
    pinMode(fan_pin, OUTPUT);
    analogWrite(fan_pin, 0);
}

void set_fan_speed(uint8_t speed)
{
    analogWrite(fan_pin, speed);
}