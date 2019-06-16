#pragma once

#include <Arduino.h>

#include "time.h"

class Task;

const Time SCHEDULER_DELAY_LIMIT = microseconds(10);
const uint32_t MAX_TASK_COUNT = 20;
const uint32_t MOVING_SUM_COUNT = 32;

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
    Time     maxExecutionTime;
    Time     totalExecutionTime;
    Time     averageExecutionTime;
};

void getCheckFuncInfo(CheckFuncInfo_t *checkFuncInfo);
void rescheduleTask(Task* task, Time newPeriod);
bool addTask(Task* task);
void setTaskEnabled(Task* task, bool newEnabledState);
void schedulerSetCalulateTaskStatistics(bool calculateTaskStatistics);
void schedulerResetTaskStatistics(Task* task);
uint16_t getAverageSystemLoadPercent();

void schedulerInit();
void scheduler();