#pragma once

#include <Arduino.h>

#include "time.h"

float* get_fft_data();

void init_filters();

void apply_filters();
void apply_attenuation();
void apply_clipping();

void init_audio();

bool rms_available(const Time& currentTime, const Time& currentDeltaTime);
bool fft_available(const Time& currentTime, const Time& currentDeltaTime);

void update_rms(const Time& currentTime);
void update_fft(const Time& currentTime);

void print_levels();
void print_rms();
void print_cpu_use();