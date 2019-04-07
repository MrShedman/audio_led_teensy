#include "audio_proc.h"

#include <Audio.h>

#include "filter.h"

const int myInput = AUDIO_INPUT_LINEIN;
//const int myInput = AUDIO_INPUT_MIC;

AudioInputI2S           audioInput;
AudioAnalyzeFFT1024     fft;
AudioAnalyzeRMS         rms;

AudioMixer4             mixer;
AudioConnection         patchCord1(audioInput, 0, mixer, 0);
AudioConnection         patchCord2(audioInput, 1, mixer, 3);
AudioConnection         patchCord3(mixer, fft);
AudioConnection         patchCord5(mixer, rms);

AudioControlSGTL5000 audioShield;

// An array to hold the 16 frequency bands
float levels_raw[16];
float levels_avg[16];
float rms_level;

float dt = 1.0 / 87.0;
float f_cut = 5.0;
lowPassFilter_t filters[16];
lowPassFilter_t rms_filter;

float* get_fft_data()
{
    return levels_avg;
}

void init_filters()
{
    for (uint8_t i = 0; i < 16; i++)
    {
        lowPassFilterInit(&filters[i], f_cut, dt);
    }
    lowPassFilterInit(&rms_filter, f_cut, dt);
}

void apply_filters()
{
    for (uint8_t i = 0; i < 16; i++)
    {
        levels_avg[i] = lowPassFilterApply(&filters[i], levels_raw[i]);
    }
    rms_level = lowPassFilterApply(&rms_filter, rms_level);
}

float smoothstep_high_pass(float x, float cut)
{
    if (x > cut)
    {
        return 1.0;
    }
    else
    {
        x /= cut;
        return x * x * (3.0 - 2.0 * x);
    }
}

float smoothstep_low_pass(float x, float cut)
{
    if (x < cut)
    {
        return 0.0;
    }
    else
    {
        x = map(x, cut, 1.0, 0.0, 1.0);
        return x * x * (3.0 - 2.0 * x);
    }
}

void apply_attenuation()
{
    for (uint8_t i = 0; i < 16; i++)
    {
        levels_avg[i] = smoothstep_low_pass(levels_avg[i], 0.0);
    } 
}

void apply_clipping()
{
    for (uint8_t i = 0; i < 16; i++)
    {
        levels_avg[i] = constrain(levels_avg[i], 0.0, 1.0);
    } 
}

void init_audio()
{
    AudioMemory(12);

    audioShield.enable();
    audioShield.inputSelect(myInput);
    audioShield.volume(1.0);
    audioShield.lineInLevel(7);

    fft.windowFunction(AudioWindowNuttall1024);//AudioWindowHanning1024);

    init_filters();
}

bool rms_available(uint32_t currentTimeUs, uint32_t currentDeltaTimeUs)
{
    return rms.available();
}

bool fft_available(uint32_t currentTimeUs, uint32_t currentDeltaTimeUs)
{
    return fft.available();
}

void update_rms(uint32_t currentTimeUs)
{
    rms_level = rms.read();
}

void update_fft(uint32_t currentTimeUs)
{
    levels_raw[0] =  fft.read(0);
    levels_raw[1] =  fft.read(1);
    levels_raw[2] =  fft.read(2, 3);
    levels_raw[3] =  fft.read(4, 6);
    levels_raw[4] =  fft.read(7, 10);
    levels_raw[5] =  fft.read(11, 15);
    levels_raw[6] =  fft.read(16, 22);
    levels_raw[7] =  fft.read(23, 32);
    levels_raw[8] =  fft.read(33, 46);
    levels_raw[9] =  fft.read(47, 66);
    levels_raw[10] = fft.read(67, 93);
    levels_raw[11] = fft.read(94, 131);
    levels_raw[12] = fft.read(132, 184);
    levels_raw[13] = fft.read(185, 257);
    levels_raw[14] = fft.read(258, 359);
    levels_raw[15] = fft.read(360, 511);

    apply_filters();
    apply_attenuation();
    apply_clipping();
}

void print_levels()
{
    for (uint8_t i = 0; i < 16; i++) 
    {
        Serial.print(levels_avg[i]);
        Serial.print(",");
    }

    Serial.println();
}

void print_rms()
{
    Serial.print(rms_level);

    Serial.println();
}

void print_cpu_use()
{
    Serial.print("fft=");
    Serial.print(fft.processorUsage());
    Serial.print(",");
    Serial.print(fft.processorUsageMax());
    Serial.print("  ");
    Serial.print("all=");
    Serial.print(AudioProcessorUsage());
    Serial.print(",");
    Serial.print(AudioProcessorUsageMax());
    Serial.print("    ");
    Serial.print("Memory: ");
    Serial.print(AudioMemoryUsage());
    Serial.print(",");
    Serial.print(AudioMemoryUsageMax());
    Serial.println();
}