//
// Created by Martynas Skrebė on 17/01/2025.
//

#include "Application.h"
#include "Tasks/ManagerTask.h"

#include "FreeRTOS.h"
#include "task.h"

void Application::start() {
    auto managerTask = ManagerTask::getInstance();
    managerTask->start();

    vTaskStartScheduler();
}
