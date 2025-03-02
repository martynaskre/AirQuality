//
// Created by Martynas Skrebė on 12/01/2025.
//

#include "SensorCalibrationTask.h"
#include "AirQuality.h"
#include "options.h"
#include "Utils/Utils.h"

SensorCalibrationTask::SensorCalibrationTask(): FreeRTOSTask("SensorCalibrationTask", 1024, 2) {}

void SensorCalibrationTask::execute() {
    while (true) {
        debug("Calibrating sensors...");

        AirQuality::calibrate();

        vTaskDelay(pdMS_TO_TICKS(SGP30_RECALIBRATE_EVERY));
    }
}
