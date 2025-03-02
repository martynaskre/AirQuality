//
// Created by Martynas Skrebė on 01/12/2024.
//

#ifndef AIRQUALITY_FREERTOSTASK_H
#define AIRQUALITY_FREERTOSTASK_H


#include "FreeRTOS.h"
#include "task.h"
#include <string>

class FreeRTOSTask {
    TaskHandle_t taskHandle;
    std::string name;
    uint32_t stackSize;
    UBaseType_t priority;

    static void taskRunner(void* params);
protected:
    virtual void execute() = 0;
public:
    explicit FreeRTOSTask(std::string name, uint32_t stackSize = 1024, UBaseType_t priority = 1);
    virtual ~FreeRTOSTask();

    void start();
    TaskHandle_t& getTaskHandle();
};


#endif //AIRQUALITY_FREERTOSTASK_H
