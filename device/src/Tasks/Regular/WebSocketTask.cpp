//
// Created by Martynas Skrebė on 17/01/2025.
//

#include <sstream>
#include <iomanip>
#include "WebSocketTask.h"
#include "AirQuality.h"

WebSocketTask::WebSocketTask(): FreeRTOSTask("WebSocketTask", ((configSTACK_DEPTH_TYPE) 2048), 1) {
    server = WebSocketServer();
}

void WebSocketTask::execute() {
    server.addSender(sendSensorData, 1000);

    server.start();
}

void WebSocketTask::sendSensorData(mg_mgr *manager, void *data) {
    auto measurement = AirQuality::getMeasurement();

    std::ostringstream payload;

    payload << std::fixed << std::setprecision(2)
            << "{"
            << "\"lux\":" << measurement.lux << ","
            << "\"temperature\":" << measurement.temperature << ","
            << "\"pressure\":" << measurement.pressure << ","
            << "\"humidity\":" << measurement.humidity << ","
            << "\"eco2\":" << measurement.eCO2 << ","
            << "\"tvoc\":" << measurement.TVOC << ","
            << "}";

    for (mg_connection *connection = manager->conns; connection != nullptr; connection = connection->next) {
        if (connection->data[0] == 'W') {
            mg_ws_printf(connection, WEBSOCKET_OP_TEXT, "%s", payload.str().c_str());
        }
    }
}
