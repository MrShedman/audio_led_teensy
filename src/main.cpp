#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#include <WS2812Serial.h>

#include "color.h"

const uint16_t numled0 = 288;
const uint16_t numled1 = 288;
const uint8_t pin0 = 8;
const uint8_t pin1 = 10;

uint8_t drawingMemory0[numled0*3];         //  3 bytes per LED
DMAMEM uint8_t displayMemory0[numled0*12]; // 12 bytes per LED

uint8_t drawingMemory1[numled1*3];         //  3 bytes per LED
DMAMEM uint8_t displayMemory1[numled1*12]; // 12 bytes per LED

WS2812Serial leds0(numled0, displayMemory0, drawingMemory0, pin0, WS2812_GRB);
WS2812Serial leds1(numled1, displayMemory1, drawingMemory1, pin1, WS2812_GRB);

//const int myInput = AUDIO_INPUT_LINEIN;
const int myInput = AUDIO_INPUT_MIC;

// Create the Audio components.  These should be created in the
// order data flows, inputs/sources -> processing -> outputs
//
AudioInputI2S          audioInput;         // audio shield: mic or line-in
AudioSynthWaveformSine sinewave;
AudioAnalyzeFFT1024    myFFT;
AudioOutputI2S         audioOutput;        // audio shield: headphones & line-out

// Connect either the live input or synthesized sine wave
AudioConnection patchCord1(audioInput, 0, myFFT, 0);
//AudioConnection patchCord1(sinewave, 0, myFFT, 0);

AudioControlSGTL5000 audioShield;

// An array to hold the 16 frequency bands
float levels_raw[16];
float levels_avg[16];

uint32_t loop_start_time = 0;
double loop_rate = 60.0;
const uint32_t loop_time_us = 1e6 / loop_rate;

uint8_t loop_counter = 0;
uint8_t num_samples = 2;
float hue = 0.0;

void spin_once()
{
    while (micros() - loop_start_time < loop_time_us);

    loop_start_time = micros();
}

void setup() 
{
    //Serial.begin(115200);
    //while(!Serial);

    // Audio connections require memory to work.  For more
    // detailed information, see the MemoryAndCpuUsage example
    AudioMemory(12);

    // Enable the audio shield and set the output volume.
    audioShield.enable();
    audioShield.inputSelect(myInput);
    audioShield.volume(1.0);

    // Configure the window algorithm to use
    myFFT.windowFunction(AudioWindowHanning1024);
    //myFFT.windowFunction(NULL);

    // Create a synthetic sine wave, for testing
    // To use this, edit the connections above
    sinewave.amplitude(0.8);
    sinewave.frequency(1034.007);

    leds0.begin();
    leds1.begin();
}

bool read_fft()
{
    bool rtn_sts = false;

    if (myFFT.available()) 
    {
        // read the 512 FFT frequencies into 16 levels
        // music is heard in octaves, but the FFT data
        // is linear, so for the higher octaves, read
        // many FFT bins together.
        levels_raw[0] +=  myFFT.read(0);
        levels_raw[1] +=  myFFT.read(1);
        levels_raw[2] +=  myFFT.read(2, 3);
        levels_raw[3] +=  myFFT.read(4, 6);
        levels_raw[4] +=  myFFT.read(7, 10);
        levels_raw[5] +=  myFFT.read(11, 15);
        levels_raw[6] +=  myFFT.read(16, 22);
        levels_raw[7] +=  myFFT.read(23, 32);
        levels_raw[8] +=  myFFT.read(33, 46);
        levels_raw[9] +=  myFFT.read(47, 66);
        levels_raw[10] += myFFT.read(67, 93);
        levels_raw[11] += myFFT.read(94, 131);
        levels_raw[12] += myFFT.read(132, 184);
        levels_raw[13] += myFFT.read(185, 257);
        levels_raw[14] += myFFT.read(258, 359);
        levels_raw[15] += myFFT.read(360, 511);

        loop_counter++;

        if (loop_counter == num_samples)
        {
            const float scale = 1.0 / (float)num_samples;
            for (uint8_t i = 0; i < 16; i++)
            {
                levels_avg[i] = levels_raw[i] * scale;
                levels_raw[i] = 0.0;
            }

            loop_counter = 0;
            rtn_sts = true;
        }      
    }

    return rtn_sts;
}

void print_levels()
{
    for (uint8_t i = 0; i < 16; i++) 
    {
        Serial.print(levels_avg[i]);
        Serial.print(",");
    }
    Serial.print(levels_avg[15]);

    Serial.println();
}

void update_leds()
{
    for (int i = 0; i < 16; i++)
    {
        uint32_t color = hsv2rgb(hue, 1.0, levels_avg[i]);

        for (int j = 0; j < 18; j++)
        {
            int idx = i * 18 + j;
            leds0.setPixel(idx, color);
            leds1.setPixel(idx, color);
        }
    }

    leds0.show();
    leds1.show();
}

void increase_hue()
{
    hue += 1;

    if (hue > 360)
    {
        hue = 0.0;
    }
}

void loop() 
{
    if (read_fft())
    {
        print_levels();

        update_leds();

        increase_hue();
    }
}