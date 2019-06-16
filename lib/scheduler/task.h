#pragma once

#include "time.h"

class Task 
{
public:

    typedef bool (checkFunction)(const Time& currentTime, const Time& currentDeltaTime);
    typedef void (taskFunction)(const Time& currentTime);

    Task(const char* taskName, checkFunction* checkFunc_ptr, taskFunction* taskFunc_ptr, Time desired_period, const uint8_t priority);

    void print_info();

    // Configuration
    uint8_t id;
    const char* taskName;
	checkFunction* checkFunc;
	taskFunction* taskFunc;
    Time desiredPeriod;      // target period of execution
    const uint8_t staticPriority;   // dynamicPriority grows in steps of this size, shouldn't be zero

    // Scheduling
    uint16_t dynamicPriority;       // measurement of how old task was last executed, used to avoid task starvation
    uint16_t taskAgeCycles;
    Time taskLatestDeltaTime;
    Time lastExecutedAt;        // last time of invocation
    Time lastSignaledAt;        // time of invocation event for event-driven tasks

    // Statistics
    Time movingSumExecutionTime;  // moving sum over 32 samples
    Time maxExecutionTime;
    Time totalExecutionTime;    // total time consumed by task since boot
};