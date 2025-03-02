//
// Created by Martynas Skrebė on 22/02/2025.
//

#include "ResourcesTask.h"
#include "Utils/Utils.h"

ResourcesTask::ResourcesTask(): FreeRTOSTask("ResetButtonTask", 1024, 5) {}

void ResourcesTask::execute() {
    while (true) {
        debug("Heap usage: %u/%u", getFreeHeap(), getTotalHeap());

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}
