#include "rate.h"

<<<<<<< 355839a92907f7fc8ef0c9cb0b38a8b4f70b8aa4
#include <Arduino.h>

Rate::Rate(const float hz)
:
m_loop_time(seconds(1.0 / hz)),
m_loop_start_time(Time::now())
=======
Rate::Rate(const float hz)
:
m_loop_time(hertz(hz)),
m_loop_clock()
>>>>>>> Add chrono lib
{

}

void Rate::sleep()
{
<<<<<<< 355839a92907f7fc8ef0c9cb0b38a8b4f70b8aa4
    while (Time::now() - m_loop_start_time < m_loop_time);

    m_loop_start_time = Time::now();
=======
    while (m_loop_clock.get_elapsed_time() < m_loop_time);

    m_loop_clock.restart();
>>>>>>> Add chrono lib
}