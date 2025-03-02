//
// Created by Martynas Skrebė on 17/01/2025.
//

#include "ManagerTask.h"
#include "Network/Network.h"
#include "Utils/Utils.h"
#include "Utils/Flash.h"
#include "Network/HTTPClient.h"
#include "options.h"

#include "Tasks/Pairing/BluetoothPairingTask.h"
#include "Tasks/Regular/WebSocketTask.h"
#include "Tasks/Regular/LogSensorDataTask.h"
#include "Tasks/Utils/BlinkLedTask.h"
#include "Tasks/Utils/TimedCommandTask.h"
#include "Tasks/Sensors/SensorCalibrationTask.h"
#include "Tasks/Sensors/SensorUpdateTask.h"

ManagerTask* ManagerTask::instance = nullptr;

ManagerTask::ManagerTask(): FreeRTOSTask("ManagerTask", 1024, 1), currentCommand(std::nullopt) {
    resetButtonTask = ResetButtonTask();
    resourcesTask = ResourcesTask();
}

ManagerTask::~ManagerTask() {
    delete instance;

    instance = nullptr;
}

ManagerTask *ManagerTask::getInstance() {
    if (instance == nullptr) {
        instance = new ManagerTask();
    }

    return instance;
}

std::optional<ManagerCommand> &ManagerTask::getCurrentCommand() {
    return currentCommand;
}

void ManagerTask::notify(ManagerCommand command) {
    xTaskNotify(getInstance()->getTaskHandle(), static_cast<uint32_t>(command), eSetValueWithOverwrite);
}

void ManagerTask::execute() {
    resetButtonTask.start();
    resourcesTask.start();

    startRegular();

    uint32_t rawCommand;

    while (true) {
        if (xTaskNotifyWait(0, 0, &rawCommand, 0) == pdTRUE) {
            auto command = static_cast<ManagerCommand>(rawCommand);

            switch (command) {
                case START_PAIRING:
                    startPairing();

                    break;
                case VALIDATE_NETWORK:
                    startNetworkValidation();

                    break;
                case START_REGULAR:
                    startRegular();

                    break;
                case RESET:
                    reset();

                    break;
            }
        }
    }
}

void ManagerTask::reset(bool hard) {
    debug("Started %s reset", hard ? "HARD" : "");

    tasks.clear();

    if (hard) {
        Flash::clear();
    }

    Network::closeStation();
    Bluetooth::stop();

    currentCommand.reset();

    debug("%s finished", hard ? "HARD reset" : "Reset");
}

void ManagerTask::startPairing() {
    reset(true);

    currentCommand = START_PAIRING;

    debug("Starting pairing");

    tasks.push_back(std::make_unique<BlinkLedTask>());
    tasks[0]->start();

    tasks.push_back(std::make_unique<BluetoothPairingTask>());
    tasks[1]->start();

    tasks.push_back(std::make_unique<TimedCommandTask>(RESET, RUN_PAIRING_FOR));
    tasks[2]->start();

    debug("Pairing started");
}

void ManagerTask::startNetworkValidation() {
    reset();

    currentCommand = VALIDATE_NETWORK;

    debug("Starting network validation");

    tasks.push_back(std::make_unique<BlinkLedTask>());
    tasks[0]->start();

    auto flashData = Flash::get();

    if (!flashData.has_value()) {
        error("No data in flash found to do network validation");

        reset();

        return;
    } else if (Network::joinNetwork(flashData->ssid, flashData->password, 30000) != ERR_OK) {
        warning("Could not connect to SSID: %s, password: %s", flashData->ssid, flashData->password);

        reset(true);

        return;
    }

    HTTPClient::destroy(SERVER_MEASUREMENTS_ENDPOINT, 3000);

    debug("Network validation successful");

    startRegular();
}

void ManagerTask::startRegular() {
    reset();

    currentCommand = START_REGULAR;

    debug("Starting regular");

    auto flashData = Flash::get();

    if (!flashData.has_value() || Network::joinNetwork(flashData->ssid, flashData->password, 30000) != ERR_OK) {
        warning("Could not connect to SSID: %s", flashData->ssid);

        reset(true);

        return;
    }

    tasks.push_back(std::make_unique<SensorCalibrationTask>());
    tasks[0]->start();

    tasks.push_back(std::make_unique<SensorUpdateTask>());
    tasks[1]->start();

    tasks.push_back(std::make_unique<WebSocketTask>());
    tasks[2]->start();

    tasks.push_back(std::make_unique<LogSensorDataTask>());
    tasks[3]->start();

    debug("Regular started");
}
