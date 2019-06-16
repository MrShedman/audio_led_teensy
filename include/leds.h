#pragma once

#include <Arduino.h>

#include "time.h"

void init_leds();

void set_hue(float h);

float get_hue();

void set_hue_auto_increase(bool flag);

bool get_hue_auto_increase();

void update_leds(const Time& currentTime);