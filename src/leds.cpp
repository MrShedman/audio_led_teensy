#include "leds.h"

#include <WS2812Serial.h>

#include "color.h"
#include "pins.h"

const uint16_t numled0 = 288;
const uint16_t numled1 = 288;

uint8_t drawingMemory0[numled0*3];         //  3 bytes per LED
DMAMEM uint8_t displayMemory0[numled0*12]; // 12 bytes per LED

uint8_t drawingMemory1[numled1*3];         //  3 bytes per LED
DMAMEM uint8_t displayMemory1[numled1*12]; // 12 bytes per LED

WS2812Serial leds0(numled0, displayMemory0, drawingMemory0, led_pin0, WS2812_GRB);
WS2812Serial leds1(numled1, displayMemory1, drawingMemory1, led_pin1, WS2812_GRB);

float hue = 0.0;

void init_leds()
{
    leds0.begin();
    leds1.begin();
}

void set_hue(float h)
{
    hue = constrain(h, 0.0, 360.0);
}

void increase_hue(float rate)
{
    hue += 0.1;

    if (hue > 360)
    {
        hue = 0.0;
    }
}

void convolve(const uint8_t *u, uint8_t smoothed[288])
{
    uint8_t leds[288];
    int32_t jC;
    int32_t jA2;
    uint32_t s;
    int32_t k;
    static const uint8_t B[36] = {  11, 16, 22, 30, 40, 52, 
                                    66, 83, 102, 122, 144, 166,
                                    187, 207, 225, 239, 249, 254, 
                                    254, 249, 239, 225, 207, 187, 
                                    166, 144, 122, 102, 83, 66, 
                                    52, 40, 30, 22, 16, 11 };

    const uint32_t window_sum = 4430;

    for (uint8_t m = 0; m < 16; m++)
    {
        for (uint8_t n = 0; n < 18; n++)
        {
            leds[m * 18 + n] = u[m];
        }
    }

    for (jC = 0; jC < 288; jC++)
    {
        jA2 = jC + 19;
        if (288 <= jA2) 
        {
            jA2 = 288;
        }

        s = 0;
        if (36 < jC + 20)
        {
            k = jC - 16;
        } 
        else
        {
            k = 1;
        }

        while (k <= jA2) 
        {
            s += leds[k - 1] * B[(jC - k) + 19];
            k++;
        }

        s /= window_sum;
        smoothed[jC] = s;
    }
}

void update_leds(float fft[16])
{
    uint8_t out[288];
    uint8_t in[16];

    for (uint8_t i = 0; i < 16; i++)
    {
        in[i] = uint8_t(fft[i] * 255.0);
    }

    convolve(in, out);

    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 18; j++)
        {
            int idx = i * 18 + j;
            rgb_t c = hsv2rgb(hue, 1.0, out[idx] / 255.0);
            leds0.setPixel(idx, c.r, c.g, c.b);
            leds1.setPixel(idx, c.r, c.g, c.b);
        }
    }

    leds0.show();
    leds1.show();
}