#pragma once

#include <Arduino.h>

float* get_fft_data();

void init_filters();
void apply_filters();

float smoothstep_high_pass(float x, float cut);
float smoothstep_low_pass(float x, float cut);

void apply_attenuation();
void apply_clipping();

void init_audio();

bool read_rms();
bool read_fft();

void print_levels();
void print_rms();
void print_cpu_use();