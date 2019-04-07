#include "rate.h"

Rate::Rate(const float hz)
:
m_rate(hz),
m_loop_time_us(1e6 / m_rate),
m_loop_start_time(micros())
{

}

void Rate::sleep()
{
    while (micros() - m_loop_start_time < m_loop_time_us);

    m_loop_start_time = micros();
}