#include "filter.h"

namespace
{
	const float TWO_PI = 6.28318530718;
}

void lowPassFilterInit(lowPassFilter_t *filter, float f_cut, float dT)
{
	filter->RC = 1.0 / (TWO_PI * f_cut);
	filter->dT = dT;
	filter->k = filter->dT / (filter->RC + filter->dT);
}

float lowPassFilterApply(lowPassFilter_t *filter, float input)
{
	filter->state = filter->state + filter->k * (input - filter->state);
	return filter->state;
}

void highPassFilterInit(highPassFilter_t *filter, float f_cut, float dT)
{
	filter->RC = 1.0 / (TWO_PI * f_cut);
	filter->dT = dT;
	filter->alpha = filter->RC / (filter->RC + filter->dT);
}

float highPassFilterApply(highPassFilter_t *filter, float input)
{
	filter->state = filter->alpha * (filter->state + input - filter->prev_input);
	filter->prev_input = input;
	return filter->state;
}