
#include <Arduino.h>

#include "audio_proc.h"
#include "connectivity.h"
#include "leds.h"
#include "thermal.h"
#include "scheduler.h"
#include "task.h"

Task fft_task("fft_task", fft_available, update_fft, hertz(87), TASK_PRIORITY_REALTIME);
Task thermal_task("thermal_task", nullptr, update_temp, hertz(temp_read_rate), TASK_PRIORITY_LOW);
Task power_task("power_task", nullptr, update_power, hertz(current_read_rate), TASK_PRIORITY_MEDIUM);

Task blynk_run_task("blynk_run_task", nullptr, update_blynk, hertz(200), TASK_PRIORITY_HIGH);
Task blynk_sync_task("blynk_sync_task", nullptr, sync_params, hertz(20), TASK_PRIORITY_MEDIUM_HIGH);

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