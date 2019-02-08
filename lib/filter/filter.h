
#pragma once

#include "Arduino.h"

typedef struct pt1Filter_s {
	float state;
	float k;
	float RC;
	float dT;
} pt1Filter_t;

typedef float(*filterApplyFnPtr)(void *filter, float input);

float nullFilterApply(void *filter, float input);

void pt1FilterInit(pt1Filter_t *filter, float f_cut, float dT);
float pt1FilterApply(pt1Filter_t *filter, float input);
float pt1FilterApply4(pt1Filter_t *filter, float input, float f_cut, float dT);