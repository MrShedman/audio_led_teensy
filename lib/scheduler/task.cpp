#include "task.h"

#include "logger.h"
#include "scheduler.h"

Task::Task(const char* taskName, checkFunction checkFunc_ptr, taskFunction taskFunc_ptr, Time desired_period, const uint8_t priority)
:
id(255),
taskName(taskName),
checkFunc(checkFunc_ptr),
taskFunc(taskFunc_ptr),
desiredPeriod(desired_period),
staticPriority(priority)
{
}

void Task::print_info()
{
    LOG_INFO("%s\n", taskName);
    LOG_INFO("ID: %d\n", id);
    //LOG_INFO("Enabled: %s\n", queueContains(this) ? "true" : "false");
    LOG_INFO("Static priority: %d\n", staticPriority);
    LOG_INFO("Desired period: %dus\n", desiredPeriod.asMicroseconds());
    LOG_INFO("Latest Delta Time: %dus\n", taskLatestDeltaTime.asMicroseconds());
    LOG_INFO("Max Execution Time: %dus\n", maxExecutionTime.asMicroseconds());
    LOG_INFO("Total Execution Time: %dus\n", totalExecutionTime.asMicroseconds());
    LOG_INFO("Avg Execution Time: %dus\n", movingSumExecutionTime / (int64_t)MOVING_SUM_COUNT);
}