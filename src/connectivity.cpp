#include "connectivity.h"

#define BLYNK_PRINT Serial

#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>

#include "pins.h"
#include "thermal.h"
#include "leds.h"

ESP8266 wifi(&Serial1);

char auth[] = "0a764907c78042b497a89d6f2afbecab";
char ssid[] = "PLUSNET-7WTF";
char pass[] = "6c2347ce5a";
char ip[] =  "192.168.1.83";

void init_blynk()
{
    Serial1.begin(115200);

    pinMode(ch_pd, OUTPUT);
    digitalWrite(ch_pd, LOW);
    delay(50);
    digitalWrite(ch_pd, HIGH);

    Blynk.begin(auth, wifi, ssid, pass, ip, 8080);
}

void update_blynk(uint32_t currentTimeUs)
{
    Blynk.run();
}

BLYNK_WRITE(V0)
{
    set_fan_speed(param.asInt());
}

BLYNK_WRITE(V1)
{
    set_hue_auto_increase(param.asInt());
}

BLYNK_WRITE(V2)
{
    set_hue(param.asInt());
}

void sync_params(uint32_t currentTimeUs)
{
    Blynk.virtualWrite(V0, (int)get_fan_speed());
    Blynk.virtualWrite(V1, get_hue_auto_increase());
    Blynk.virtualWrite(V2, (int)get_hue());

    const power_stats_t p = get_power_stats();
    Blynk.virtualWrite(V3, p.voltage_mv);
    Blynk.virtualWrite(V4, p.current_ma);
    Blynk.virtualWrite(V5, p.power_w);
    Blynk.virtualWrite(V6, p.total_energy_wh);
}