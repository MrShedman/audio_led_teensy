#pragma once

#include <Arduino.h>

void init_leds();

void set_hue(float h);

void increase_hue(float rate);

void convolve(const uint8_t *u, uint8_t smoothed[288]);

void update_leds(float fft[16]);