#pragma once

struct lowPassFilter_t 
{
	float state;
	float k;
	float RC;
	float dT;
};

struct highPassFilter_t 
{
	float state;
	float prev_input;
	float alpha;
	float RC;
	float dT;
};

void lowPassFilterInit(lowPassFilter_t *filter, float f_cut, float dT);
float lowPassFilterApply(lowPassFilter_t *filter, float input);

void highPassFilterInit(highPassFilter_t *filter, float f_cut, float dT);
float highPassFilterApply(highPassFilter_t *filter, float input);