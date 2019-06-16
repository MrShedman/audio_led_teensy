#pragma once

#include <Arduino.h>

#include "time.h"

void init_blynk();

void update_blynk(const Time& currentTime);

void sync_params(const Time& currentTime);