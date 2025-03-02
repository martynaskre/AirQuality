//
// Created by Martynas Skrebė on 07/02/2025.
//

#include "BluetoothPairingTask.h"
#include "btstack.h"
#include "Tasks/ManagerTask.h"
#include "Utils/Utils.h"
#include "Network/Network.h"
#include "options.h"
#include "Utils/Flash.h"

BluetoothPairingTask::BluetoothPairingTask(): FreeRTOSTask("BluetoothPairingTask", ((configSTACK_DEPTH_TYPE) 2048), 1), networkCredentials(std::nullopt), respondTo(std::nullopt) {}

void BluetoothPairingTask::execute() {
    Bluetooth::setCredentialsHandler(BluetoothPairingTask::credentialsHandler, this);

    if (Bluetooth::start() != ERR_OK) {
        vTaskDelay(pdMS_TO_TICKS(1000));

        ManagerTask::notify(RESET);

        return;
    }

    while (true) {
        vTaskDelay(pdMS_TO_TICKS(100));

        if (networkCredentials.has_value() && respondTo.has_value()) {
            bool connectionOk = Network::testNetwork(networkCredentials->ssid, networkCredentials->password, 30000) == ERR_OK;

            debug("Test connection to network: %s", connectionOk ? "successful" : "bad credentials");

            if (connectionOk) {
                FlashData flashData{};

                strncpy(flashData.ssid, networkCredentials->ssid, sizeof(flashData.ssid) - 1);
                strncpy(flashData.password, networkCredentials->password, sizeof(flashData.password) - 1);

                printf("write flash start \n");
                Flash::set(flashData);
                printf("write flash end \n");

                const auto* clientResponse = reinterpret_cast<const uint8_t*>(SERIAL_NUMBER);

                att_server_notify(respondTo.value(), NETWORK_STATUS_HANDLE, clientResponse, strlen(SERIAL_NUMBER));

                // allow response to stream
                vTaskDelay(pdMS_TO_TICKS(500));

                break;
            } else {
                uint8_t clientResponse = 0x00;

                att_server_notify(respondTo.value(), NETWORK_STATUS_HANDLE, &clientResponse, sizeof(clientResponse));

                networkCredentials.reset();
                respondTo.reset();
            }
        }
    }

    ManagerTask::notify(VALIDATE_NETWORK);
}

void BluetoothPairingTask::credentialsHandler(hci_con_handle_t connectionHandle, uint8_t *buffer, uint16_t bufferSize, void *data) {
    auto* task = static_cast<BluetoothPairingTask*>(data);

    if (task == nullptr || task->networkCredentials.has_value() || task->respondTo.has_value()) {
        return;
    }

    if (bufferSize == sizeof(NetworkCredentials)) {
        NetworkCredentials networkCredentials = *reinterpret_cast<const NetworkCredentials*>(buffer);

        networkCredentials.ssid[32] = '\0';
        networkCredentials.password[63] = '\0';

        task->networkCredentials = networkCredentials;
        task->respondTo = connectionHandle;

        debug("Received credentials: %s, %s", networkCredentials.ssid, networkCredentials.password);
    } else {
        debug("Invalid data size received: %u bytes, expected: %u", bufferSize, sizeof(NetworkCredentials));
    }
}
