#pragma once

<<<<<<< 355839a92907f7fc8ef0c9cb0b38a8b4f70b8aa4
#include "time.h"
=======
#include "clock.h"
>>>>>>> Add chrono lib

class Rate
{
public:

    Rate(const float hz);

    void sleep();

private:

    const Time m_loop_time;
<<<<<<< 355839a92907f7fc8ef0c9cb0b38a8b4f70b8aa4
    Time m_loop_start_time;
=======
    Clock m_loop_clock;
>>>>>>> Add chrono lib
};