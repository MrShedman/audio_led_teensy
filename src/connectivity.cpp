#include "connectivity.h"

#define BLYNK_PRINT Serial

#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>

#include <pins.h>

ESP8266 wifi(&Serial1);

char auth[] = "0a764907c78042b497a89d6f2afbecab";
char ssid[] = "PLUSNET-7WTF";
char pass[] = "6c2347ce5a";
char ip[] =  "192.168.1.83";

const float blynk_run_rate_hz = 10.0;
elapsedMillis blynk_run_timer;

void init_blynk()
{
    Serial1.begin(115200);

    pinMode(ch_pd, OUTPUT);
    digitalWrite(ch_pd, LOW);
    delay(50);
    digitalWrite(ch_pd, HIGH);

    Blynk.begin(auth, wifi, ssid, pass, ip, 8080);
}

void run_blynk()
{
    if (blynk_run_timer > blynk_run_rate_hz * 1000)
    {
        Blynk.run();
        blynk_run_timer = 0;
    }
}