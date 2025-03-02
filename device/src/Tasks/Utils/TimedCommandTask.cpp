//
// Created by Martynas Skrebė on 29/01/2025.
//

#include "TimedCommandTask.h"

TimedCommandTask::TimedCommandTask(ManagerCommand command, uint64_t delay): FreeRTOSTask("TimedCommandTask", 1024, 6),
    command(command), delay(delay) {}

void TimedCommandTask::execute() {
    vTaskDelay(pdMS_TO_TICKS(delay));

    ManagerTask::notify(command);
}