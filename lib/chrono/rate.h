#pragma once

#include "time.h"

class Rate
{
public:

    Rate(const float hz);

    void sleep();

private:

    const Time m_loop_time;
    Time m_loop_start_time;
};