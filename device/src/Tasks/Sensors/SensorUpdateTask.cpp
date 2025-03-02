//
// Created by Martynas Skrebė on 12/01/2025.
//

#include "SensorUpdateTask.h"
#include "AirQuality.h"
#include "options.h"

SensorUpdateTask::SensorUpdateTask(): FreeRTOSTask("SensorUpdateTask", 1024, 3) {}

void SensorUpdateTask::execute() {
    while (true) {
        AirQuality::update();

        vTaskDelay(pdMS_TO_TICKS(UPDATE_SENORS_EVERY));
    }
}