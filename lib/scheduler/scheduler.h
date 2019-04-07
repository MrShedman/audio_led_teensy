#pragma once

#include <Arduino.h>

const uint32_t SCHEDULER_DELAY_LIMIT_US = 10u;
const uint32_t MAX_TASK_COUNT = 20;
const uint32_t MOVING_SUM_COUNT = 32;

constexpr uint32_t TASK_PERIOD_HZ(const uint32_t hz)
{
    return 1000000 / hz;
}

constexpr uint32_t TASK_PERIOD_MS(const uint32_t ms) 
{
    return ms * 1000;
}

constexpr uint32_t TASK_PERIOD_US(const uint32_t us)
{
    return us;
}

enum TaskPriority_e 
{
    TASK_PRIORITY_IDLE = 0,     // Disables dynamic scheduling, task is executed only if no other task is active this cycle
    TASK_PRIORITY_LOW = 1,
    TASK_PRIORITY_MEDIUM = 3,
    TASK_PRIORITY_MEDIUM_HIGH = 4,
    TASK_PRIORITY_HIGH = 5,
    TASK_PRIORITY_REALTIME = 6,
    TASK_PRIORITY_MAX = 255
};

struct CheckFuncInfo_t
{
    uint32_t     maxExecutionTime;
    uint32_t     totalExecutionTime;
    uint32_t     averageExecutionTime;
};

struct TaskInfo_t
{
    const char * taskName;
    uint8_t      id;
    bool         isEnabled;
    uint8_t      staticPriority;
    uint32_t     desiredPeriod;
    uint32_t     latestDeltaTime;
    uint32_t     maxExecutionTime;
    uint32_t     totalExecutionTime;
    uint32_t     averageExecutionTime;
};

struct Task_t 
{
    typedef bool (checkFunctionPtr)(uint32_t currentTimeUs, uint32_t currentDeltaTimeUs);
    typedef void (taskFunctionPtr)(uint32_t currentTimeUs);

    Task_t(const char* taskName, checkFunctionPtr* checkFunc_ptr, taskFunctionPtr* taskFunc_ptr, uint32_t desired_period, const uint8_t priority)
    :
    id(255),
    taskName(taskName),
    checkFunc(checkFunc_ptr),
    taskFunc(taskFunc_ptr),
    desiredPeriod(desired_period),
    staticPriority(priority)
    {

    }
  
    // Configuration
    uint8_t id;
    const char* taskName;
	checkFunctionPtr* checkFunc;
	taskFunctionPtr* taskFunc;
    uint32_t desiredPeriod;      // target period of execution
    const uint8_t staticPriority;   // dynamicPriority grows in steps of this size, shouldn't be zero

    // Scheduling
    uint16_t dynamicPriority;       // measurement of how old task was last executed, used to avoid task starvation
    uint16_t taskAgeCycles;
    uint32_t taskLatestDeltaTime;
    uint32_t lastExecutedAt;        // last time of invocation
    uint32_t lastSignaledAt;        // time of invocation event for event-driven tasks

    // Statistics
    uint32_t movingSumExecutionTime;  // moving sum over 32 samples
    uint32_t maxExecutionTime;
    uint32_t totalExecutionTime;    // total time consumed by task since boot
};

void printTaskInfo(Task_t* task);
void getCheckFuncInfo(CheckFuncInfo_t *checkFuncInfo);
void getTaskInfo(Task_t* task, TaskInfo_t *taskInfo);
void rescheduleTask(Task_t* task, uint32_t newPeriodMicros);
bool addTask(Task_t* task);
void setTaskEnabled(Task_t* task, bool newEnabledState);
uint32_t getTaskDeltaTime(Task_t* task);
void schedulerSetCalulateTaskStatistics(bool calculateTaskStatistics);
void schedulerResetTaskStatistics(Task_t* task);
uint16_t getAverageSystemLoadPercent();

void schedulerInit();
void scheduler();
