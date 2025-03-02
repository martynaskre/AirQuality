//
// Created by Martynas Skrebė on 01/12/2024.
//

#include "FreeRTOSTask.h"
#include <utility>

FreeRTOSTask::FreeRTOSTask(std::string name, uint32_t stackSize, UBaseType_t priority):
    name(std::move(name)), stackSize(stackSize), priority(priority), taskHandle(nullptr) {}

FreeRTOSTask::~FreeRTOSTask() {
    if (taskHandle != nullptr) {
        vTaskDelete(taskHandle);
        taskHandle = nullptr;
    }
}

void FreeRTOSTask::taskRunner(void *params) {
    auto* instance = static_cast<FreeRTOSTask*>(params);

    if (instance) {
        instance->execute();

        instance->taskHandle = nullptr;
        vTaskDelete(nullptr);
    }
}

void FreeRTOSTask::start() {
    if (taskHandle == nullptr) {
        xTaskCreate(
                taskRunner,
                name.c_str(),
                stackSize,
                this,
                priority,
                &taskHandle
        );
    }
}

TaskHandle_t& FreeRTOSTask::getTaskHandle() {
    return taskHandle;
}
