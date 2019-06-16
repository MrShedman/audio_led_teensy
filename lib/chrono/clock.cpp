#include "clock.h"

Clock::Clock()
:
m_start_time(Time::now())
{

}

Time Clock::get_elapsed_time() const
{
    return Time::now() - m_start_time;
}

Time Clock::restart()
{
    const Time now = Time::now();
    const Time elapsed = now - m_start_time;
    m_start_time = now;

    return elapsed;
}