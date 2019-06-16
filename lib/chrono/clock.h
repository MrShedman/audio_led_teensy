#pragma once

#include "time.h"

class Clock
{
public:

    Clock();

    Time get_elapsed_time() const;

    Time restart();

private:

    Time m_start_time;
};