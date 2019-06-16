#include "thermal.h"

#include "pins.h"
#include "filter.h"
#include "pid.h"

uint8_t fan_speed = 0;
lowPassFilter_t temp_filter;
const float temp_read_rate = 50.0;
const float temp_cut_off_hz = 1.0;
const float temp_setpoint = 30.0;
float temp_degrees = 0.0;

lowPassFilter_t current_filter;
const float current_read_rate = 200.0;
const float current_cut_off_hz = 1.0;

power_stats_t power_stats;

PID pid;

struct temp_sensor_t
{
    uint8_t pin;
    float degrees;
    lowPassFilter_t lpf;
};

const uint8_t num_temp_sensors = 3;

temp_sensor_t temp_sensors[num_temp_sensors];

void init_thermal_manager()
{   
    pinMode(fan_pin, OUTPUT);
    analogWrite(fan_pin, fan_speed);

    analogReadResolution(12);
    analogReadAveraging(32);

    for (uint8_t i = 0; i < num_temp_sensors; ++i)
    {
        lowPassFilterInit(&temp_sensors[i].lpf, temp_cut_off_hz, 1.0 / temp_read_rate);
    }

    lowPassFilterInit(&current_filter, current_cut_off_hz, 1.0 / current_read_rate);

    pidProfile_t prof;
    prof.kp = 10.0;
    prof.ki = 1.0;
    prof.kd = 0.0;
    prof.max_i = 128.0;
    prof.max_out = 255.0;

    pid.init(prof);
}

void set_fan_speed(uint8_t speed)
{
    fan_speed = speed;
    analogWrite(fan_pin, fan_speed);
}

uint8_t get_fan_speed()
{
    return fan_speed;
}

float get_temp()
{
    return temp_degrees;
}

const power_stats_t& get_power_stats()
{
    return power_stats;
}

float adc_to_voltage(const uint8_t pin)
{
    return map((float)analogRead(pin), 0.0, 4096.0, 0.0, 3.3);
}

float adc_to_degrees(const uint8_t pin)
{
    const float v = adc_to_voltage(pin);

    return (v - 0.5) * 100.0;
}

float adc_to_current()
{
    const float v = adc_to_voltage(current_pin);    
    
    return (73.3 * (v / 3.3) - 36.7) * 1000.0;
}

// @todo fix voltage divider
float adc_to_vin()
{
    const float R1 = 2.0;
    const float R2 = 2.0;
    const float v_div = R2 / (R1 + R2);

    const float v = adc_to_voltage(voltage_pin);

    return v / v_div;
}

void update_temp(const Time& currentTime)
{
    temp_degrees = 0.0;

    for (uint8_t i = 0; i < num_temp_sensors; ++i)
    {
        temp_sensors[i].degrees = lowPassFilterApply(&temp_sensors[i].lpf, adc_to_degrees(temp_sensors[i].pin));
        temp_degrees += temp_sensors[i].degrees;
    }

    temp_degrees /= (float)num_temp_sensors;

    const float pid_out = pid.update(temp_degrees, temp_setpoint);

    if (pid_out > 0.0)
    {
        set_fan_speed(pid_out);
    }
    else
    {
        set_fan_speed(0);
        pid.reset();
    }    
}

void update_power(const Time& currentTime)
{
    const float prev_current_ma = power_stats.current_ma;
    const float current_ma = lowPassFilterApply(&current_filter, adc_to_current());
    const float voltage_mv = adc_to_vin() * 1000.0;
    const float dt = 1.0 / current_read_rate;

    const float coloumbs = 0.5 * dt * (prev_current_ma + current_ma);

    power_stats.current_ma = current_ma;
    power_stats.voltage_mv = voltage_mv;
    power_stats.power_w = (voltage_mv * current_ma) / 1e6;
    power_stats.total_energy_wh += (coloumbs * voltage_mv) / 1e6 / 3600.0;
}