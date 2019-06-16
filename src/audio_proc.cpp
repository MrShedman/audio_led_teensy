#include "audio_proc.h"

#include <Audio.h>

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
audio_level_t levels[16];

float rms_level;
lowPassFilter_t rms_filter;

float dt = 1.0 / 87.0;
float f_cut = 5.0;

audio_level_t* get_fft_data()
{
    return levels;
}

void init_filters()
{
    for (uint8_t i = 0; i < 16; i++)
    {
        lowPassFilterInit(&levels[i].filter, f_cut, dt);
    }
    lowPassFilterInit(&rms_filter, f_cut, dt);
}

void apply_filters()
{
    for (uint8_t i = 0; i < 16; i++)
    {
        levels[i].avg = lowPassFilterApply(&levels[i].filter, levels[i].raw);
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
        levels[i].avg = smoothstep_low_pass(levels[i].avg, 0.0);
    } 
}

void apply_clipping()
{
    for (uint8_t i = 0; i < 16; i++)
    {
        levels[i].avg = constrain(levels[i].avg, 0.0, 1.0);
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

bool rms_available(const Time& currentTime, const Time& currentDeltaTime)
{
    return rms.available();
}

bool fft_available(const Time& currentTime, const Time& currentDeltaTime)
{
    return fft.available();
}

void update_rms()
{
    rms_level = rms.read();
}

void update_fft()
{
    levels[0].raw =  fft.read(0);
    levels[1].raw =  fft.read(1);
    levels[2].raw =  fft.read(2, 3);
    levels[3].raw =  fft.read(4, 6);
    levels[4].raw =  fft.read(7, 10);
    levels[5].raw =  fft.read(11, 15);
    levels[6].raw =  fft.read(16, 22);
    levels[7].raw =  fft.read(23, 32);
    levels[8].raw =  fft.read(33, 46);
    levels[9].raw =  fft.read(47, 66);
    levels[10].raw = fft.read(67, 93);
    levels[11].raw = fft.read(94, 131);
    levels[12].raw = fft.read(132, 184);
    levels[13].raw = fft.read(185, 257);
    levels[14].raw = fft.read(258, 359);
    levels[15].raw = fft.read(360, 511);

    apply_filters();
    apply_attenuation();
    apply_clipping();
}

void print_levels()
{
    for (uint8_t i = 0; i < 16; i++) 
    {
        Serial.print(levels[i].avg);
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