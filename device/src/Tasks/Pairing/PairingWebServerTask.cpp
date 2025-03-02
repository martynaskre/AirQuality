//
// Created by Martynas Skrebė on 01/12/2024.
//

#include "PairingWebServerTask.h"
#include "Utils/Flash.h"
#include "AirQuality.h"
#include "Tasks/ManagerTask.h"
#include "Network/Network.h"

PairingWebServerTask::PairingWebServerTask(): FreeRTOSTask("PairingWebServerTask", ((configSTACK_DEPTH_TYPE) 2048), 1) {
    httpServer = HTTPServer();
    credentialsEntered = false;
}

void PairingWebServerTask::execute() {
    httpServer.get("/sensors", handleSensorsEndpoint);
    httpServer.post("/wifi", handleWifiEndpoint, this);

    httpServer.startListen();

    while (true) {
        httpServer.update();

        if (xTaskNotifyWait(0, 0, nullptr, 0) == pdTRUE) {
            TickType_t endTime = xTaskGetTickCount() + pdMS_TO_TICKS(500);

            while (xTaskGetTickCount() < endTime) {
                httpServer.update();

                vTaskDelay(pdMS_TO_TICKS(100));
            }

            break;
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }

    ManagerTask::notify(VALIDATE_NETWORK);
}

void PairingWebServerTask::handleSensorsEndpoint(mg_connection *connection, mg_http_message *httpMessage, void *data) {
    auto measurement = AirQuality::getMeasurement();

    mg_http_reply(connection, 200, "Content-Type: application/json\r\n",
                  R"({"temperature": %.2f, "pressure": %.2f, "humidity": %.2f })",
                  measurement.temperature, measurement.pressure, measurement.humidity);
}

void PairingWebServerTask::handleWifiEndpoint(mg_connection *connection, mg_http_message *httpMessage, void *data) {
    auto* task = static_cast<PairingWebServerTask*>(data);

    if (task == nullptr) {
        mg_http_reply(connection, 500, "Content-Type: application/json\r\n",
                      "{%m:%m}\n", MG_ESC("status"), MG_ESC("error"));

        return;
    } else if (task->credentialsEntered) {
        mg_http_reply(connection, 500, "Content-Type: application/json\r\n",
                      "{%m:%m}\n", MG_ESC("status"), MG_ESC("credentials_entered"));

        return;
    }

    const char *extractedSsid = mg_json_get_str(httpMessage->body, "$.ssid");
    const char *extractedPassword = mg_json_get_str(httpMessage->body, "$.password");

    if (extractedSsid != nullptr && extractedPassword != nullptr) {
        if (strlen(extractedSsid) <= 33 && strlen(extractedPassword) <= 64) {
            FlashData flashData{};

            strncpy(flashData.ssid, extractedSsid, sizeof(flashData.ssid) - 1);
            strncpy(flashData.password, extractedPassword, sizeof(flashData.password) - 1);

            Flash::set(flashData);

            task->credentialsEntered = true;

            mg_http_reply(connection, 200, "Content-Type: application/json\r\n",
                          "{%m:%m}\n", MG_ESC("status"), MG_ESC("success"));

            xTaskNotifyGive(task->getTaskHandle());

            return;
        }
    }

    mg_http_reply(connection, 422, "Content-Type: application/json\r\n",
                  "{%m:%m}\n", MG_ESC("status"), MG_ESC("error"));
}
