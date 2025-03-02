//
// Created by Martynas Skrebė on 17/01/2025.
//

#include <sstream>
#include <iomanip>
#include "LogSensorDataTask.h"
#include "Network/HTTPClient.h"
#include "options.h"
#include "Utils/Utils.h"
#include "AirQuality.h"

LogSensorDataTask::LogSensorDataTask(): FreeRTOSTask("LogSensorDataTask", ((configSTACK_DEPTH_TYPE) 2048), 2) {}

void LogSensorDataTask::execute() {
    // allow sensors to calibrate
    vTaskDelay(pdMS_TO_TICKS(30 * 1000));

    while (true) {
        debug("Logging sensor data to server");

        auto measurement = AirQuality::getMeasurement();

        std::ostringstream payload;

        payload << std::fixed << std::setprecision(2)
                << "{"
                << "\"proximity\":" << measurement.proximity << ","
                << "\"lux\":" << measurement.lux << ","
                << "\"temperature\":" << measurement.temperature << ","
                << "\"pressure\":" << measurement.pressure << ","
                << "\"humidity\":" << measurement.humidity << ","
                << "\"eco2\":" << measurement.eCO2 << ","
                << "\"tvoc\":" << measurement.TVOC << ","
                << "\"raw_h2\":" << measurement.rawH2 << ","
                << "\"raw_ethanol\":" << measurement.rawEthanol << ","
                << "\"calibrated\":" << (measurement.calibrated ? "true" : "false")
                << "}";

        HTTPClient::post(SERVER_MEASUREMENTS_ENDPOINT, payload.str(), 3000);

        vTaskDelay(pdMS_TO_TICKS(LOG_DATA_EVERY));
    }
}