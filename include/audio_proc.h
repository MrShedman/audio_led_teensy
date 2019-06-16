#pragma once

#include <Arduino.h>

#include "time.h"
#include "filter.h"

struct audio_level_t
{
    float raw;
    float avg;
    lowPassFilter_t filter;
};

audio_level_t* get_fft_data();

void init_filters();

void apply_filters();
void apply_attenuation();
void apply_clipping();

void init_audio();

bool rms_available(const Time& currentTime, const Time& currentDeltaTime);
bool fft_available(const Time& currentTime, const Time& currentDeltaTime);

void update_rms();
void update_fft();

void print_levels();
void print_rms();
void print_cpu_use();