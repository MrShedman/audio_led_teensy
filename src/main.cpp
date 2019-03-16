
#include <Arduino.h>
#include <Audio.h>

#include <WS2812Serial.h>

#include "color.h"
#include "pins.h"
#include "filter.h"

#define BLYNK_PRINT Serial

#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>

char auth[] = "0a764907c78042b497a89d6f2afbecab";
char ssid[] = "PLUSNET-7WTF";
char pass[] = "6c2347ce5a";
char ip[] =  "192.168.1.83";

ESP8266 wifi(&Serial1);

const uint16_t numled0 = 288;
const uint16_t numled1 = 288;

uint8_t drawingMemory0[numled0*3];         //  3 bytes per LED
DMAMEM uint8_t displayMemory0[numled0*12]; // 12 bytes per LED

uint8_t drawingMemory1[numled1*3];         //  3 bytes per LED
DMAMEM uint8_t displayMemory1[numled1*12]; // 12 bytes per LED

WS2812Serial leds0(numled0, displayMemory0, drawingMemory0, led_pin0, WS2812_GRB);
WS2812Serial leds1(numled1, displayMemory1, drawingMemory1, led_pin1, WS2812_GRB);

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
pt1Filter_t filters[16];
pt1Filter_t rms_filter;

uint32_t loop_start_time = 0;
double loop_rate = 60.0;
const uint32_t loop_time_us = 1e6 / loop_rate;

float hue = 0.0;

float clip(float n, float lower, float upper) 
{
    return max(lower, min(n, upper));
}

void init_filters()
{
    for (uint8_t i = 0; i < 16; i++)
    {
        pt1FilterInit(&filters[i], f_cut, dt);
    }
    pt1FilterInit(&rms_filter, f_cut, dt);
}

void apply_filters()
{
    for (uint8_t i = 0; i < 16; i++)
    {
        levels_avg[i] = pt1FilterApply(&filters[i], levels_raw[i]);
    }
    rms_level = pt1FilterApply(&rms_filter, rms_level);
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
        levels_avg[i] = smoothstep_low_pass(levels_avg[i], 0.1);
    } 
}

void spin_once()
{
    while (micros() - loop_start_time < loop_time_us);

    loop_start_time = micros();
}

void set_fan_speed(uint8_t speed)
{
    analogWrite(fan_pin, speed);
}

BLYNK_WRITE(V0)
{
    float pinValue = param.asInt();
    set_fan_speed(pinValue);
    Serial.println(pinValue);
}

BLYNK_WRITE(V1)
{
    int pinValue = param.asInt();

    Serial.println(pinValue);
}

BLYNK_WRITE(V2)
{
    float pinValue = param.asInt();
    hue = pinValue;
    Serial.println(pinValue);
}

void init_audio()
{
    AudioMemory(12);

    audioShield.enable();
    audioShield.inputSelect(myInput);
    audioShield.volume(1.0);
    audioShield.lineInLevel(7);

    fft.windowFunction(AudioWindowNuttall1024);//AudioWindowHanning1024);
}

void init_blynk()
{
    const uint8_t ch_pd = 2;
    pinMode(ch_pd, OUTPUT);
    digitalWrite(ch_pd, LOW);
    delay(300);
    digitalWrite(ch_pd, HIGH);

    // Set ESP8266 baud rate
    Serial1.begin(115200);
    delay(10);

    Blynk.begin(auth, wifi, ssid, pass, ip, 8080);
}

void init_leds()
{
    leds0.begin();
    leds1.begin();
}

void setup() 
{
    init_blynk();

    init_filters();

    init_audio();

    init_leds();

    pinMode(fan_pin, OUTPUT);
    analogWrite(fan_pin, 0);
}

bool read_rms()
{
    bool rtn_sts = false;

    if (rms.available()) 
    {
        rms_level = rms.read();
    }

    return rtn_sts;
}

bool read_fft()
{
    bool rtn_sts = false;

    if (fft.available()) 
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

        rtn_sts = true;      
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

void update_leds()
{
    uint8_t out[288];
    uint8_t in[16];

    for (uint8_t i = 0; i < 16; i++)
    {
        levels_avg[i] = clip(levels_avg[i], 0.0, 1.0);
        in[i] = uint8_t(levels_avg[i] * 255.0);
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

void increase_hue()
{
    hue += 0.1;

    if (hue > 360)
    {
        hue = 0.0;
    }
}

void loop() 
{
    if (read_fft())
    {
        //print_rms();
        read_rms();


        //print_cpu_use();
        //print_levels();


        update_leds();

        //increase_hue();
    }

    Blynk.run();
}