
#include <Arduino.h>

#include "audio_proc.h"
#include "connectivity.h"
#include "leds.h"
#include "thermal.h"
#include "scheduler.h"

Task_t fft_task("fft_task", fft_available, update_fft, TASK_PERIOD_HZ(87), TASK_PRIORITY_REALTIME);
Task_t thermal_task("thermal_task", nullptr, update_temp, TASK_PERIOD_HZ(temp_read_rate), TASK_PRIORITY_LOW);
Task_t power_task("power_task", nullptr, update_power, TASK_PERIOD_HZ(current_read_rate), TASK_PRIORITY_MEDIUM);

Task_t blynk_run_task("blynk_run_task", nullptr, update_blynk, TASK_PERIOD_HZ(200), TASK_PRIORITY_HIGH);
Task_t blynk_sync_task("blynk_sync_task", nullptr, sync_params, TASK_PERIOD_HZ(20), TASK_PRIORITY_MEDIUM_HIGH);

void init_scheduler()
{
    schedulerInit();
    addTask(&fft_task);
    addTask(&thermal_task);
    addTask(&power_task);
    addTask(&blynk_run_task);
    addTask(&blynk_sync_task);

    setTaskEnabled(&fft_task, true);
    setTaskEnabled(&thermal_task, true);
    setTaskEnabled(&power_task, true);
    setTaskEnabled(&blynk_run_task, true);
    setTaskEnabled(&blynk_sync_task, true);
}

void setup() 
{
    init_blynk();

    init_audio();

    init_leds();

    init_thermal_manager();

    init_scheduler();
}

void loop() 
{
    scheduler();
}