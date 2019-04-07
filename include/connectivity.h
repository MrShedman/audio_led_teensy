#pragma once

#include <Arduino.h>

void init_blynk();

void update_blynk(uint32_t currentTimeUs);

void sync_params(uint32_t currentTimeUs);