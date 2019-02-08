
#include "filter.h"

#define M_LN2_FLOAT 0.69314718055994530942f
#define M_PI_FLOAT  3.14159265358979323846f

// NULL filter

float nullFilterApply(void *filter, float input)
{
	return input;
}

// PT1 Low Pass filter

void pt1FilterInit(pt1Filter_t *filter, float f_cut, float dT)
{
	filter->RC = 1.0f / (2.0f * M_PI_FLOAT * f_cut);
	filter->dT = dT;
	filter->k = filter->dT / (filter->RC + filter->dT);
}

float pt1FilterApply(pt1Filter_t *filter, float input)
{
	filter->state = filter->state + filter->k * (input - filter->state);
	return filter->state;
}

float pt1FilterApply4(pt1Filter_t *filter, float input, float f_cut, float dT)
{
	// Pre calculate and store RC
	if (!filter->RC) {
		filter->RC = 1.0f / (2.0f * M_PI_FLOAT * f_cut);
		filter->dT = dT;
		filter->k = filter->dT / (filter->RC + filter->dT);
	}

	filter->state = filter->state + filter->k * (input - filter->state);

	return filter->state;
}