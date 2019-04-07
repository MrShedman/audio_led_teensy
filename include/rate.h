#pragma once

#include <Arduino.h>

class Rate
{
public:

    Rate(const float hz);

    void sleep();

private:

    const float m_rate;
    const uint32_t m_loop_time_us;
    uint32_t m_loop_start_time;
};