//
// Created by Martynas Skrebė on 29/01/2025.
//

#ifndef AIRQUALITY_BLINKLEDTASK_H
#define AIRQUALITY_BLINKLEDTASK_H


#include "Tasks/FreeRTOSTask.h"

class BlinkLedTask: public FreeRTOSTask {
    uint64_t delay;
protected:
    void execute() override;
public:
    explicit BlinkLedTask(uint64_t delay = 500);
    ~BlinkLedTask() override;
};


#endif //AIRQUALITY_BLINKLEDTASK_H
