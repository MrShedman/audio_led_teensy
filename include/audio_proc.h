#pragma once

#include <Arduino.h>

float* get_fft_data();

void init_filters();

void apply_filters();
void apply_attenuation();
void apply_clipping();

void init_audio();

bool rms_available(uint32_t currentTimeUs, uint32_t currentDeltaTimeUs);
bool fft_available(uint32_t currentTimeUs, uint32_t currentDeltaTimeUs);

void update_rms(uint32_t currentTimeUs);
void update_fft(uint32_t currentTimeUs);

void print_levels();
void print_rms();
void print_cpu_use();