#pragma once

#include <Arduino.h>

extern const float temp_read_rate;
extern const float current_read_rate;

struct power_stats_t
{
    float current_ma;
    float voltage_mv;
    float power_w;

    float total_energy_wh;
};

void init_thermal_manager();

void set_fan_speed(uint8_t speed);

uint8_t get_fan_speed();

float get_temp();

power_stats_t get_power_stats();

void update_temp(uint32_t currentTimeUs);

void update_power(uint32_t currentTimeUs);