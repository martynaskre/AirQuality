//
// Created by Martynas Skrebė on 01/12/2024.
//

#include "ResetButtonTask.h"
#include "pico/stdlib.h"
#include "options.h"
#include "Utils/Flash.h"
#include "Tasks/ManagerTask.h"

ResetButtonTask::ResetButtonTask(): FreeRTOSTask("ResetButtonTask", 1024, 4) {}

void ResetButtonTask::execute() {
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);

    uint64_t pressStartTime = 0;
    bool isPressed = false;

    while (true) {
        if (!gpio_get(BUTTON_PIN)) {
            if (!isPressed) {
                pressStartTime = to_ms_since_boot(get_absolute_time());
                isPressed = true;
            } else {
                uint64_t elapsedTime = to_ms_since_boot(get_absolute_time()) - pressStartTime;

                if (elapsedTime >= LONG_PRESS_FOR && canEnterPairing()) {
                    ManagerTask::notify(START_PAIRING);

                    isPressed = false;
                }
            }
        } else {
            isPressed = false;
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

bool ResetButtonTask::canEnterPairing() {
    auto instance = ManagerTask::getInstance();

    if (!instance->getCurrentCommand().has_value()) {
        return true;
    }

    return instance->getCurrentCommand().value() != ManagerCommand::START_PAIRING &&
            instance->getCurrentCommand().value() != ManagerCommand::VALIDATE_NETWORK;
}
