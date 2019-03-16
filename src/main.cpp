
#include <Arduino.h>

#include "color.h"
#include "pins.h"
#include "filter.h"
#include "connectivity.h"
#include "leds.h"
#include "audio_proc.h"
#include "utils.h"

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
    set_hue(param.asInt());
}

void setup() 
{
    init_blynk();

    init_audio();

    init_leds();

    init_fan();
}

void loop() 
{
    if (read_fft())
    {
        //print_rms();
        read_rms();

        //print_cpu_use();
        //print_levels();

        update_leds(get_fft_data());
    }

    run_blynk();
}