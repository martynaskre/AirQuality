//
// Created by Martynas Skrebė on 01/12/2024.
//

#ifndef AIRQUALITY_RESETBUTTONTASK_H
#define AIRQUALITY_RESETBUTTONTASK_H


#include "Tasks/FreeRTOSTask.h"

class ResetButtonTask: public FreeRTOSTask {
    static bool canEnterPairing();
protected:
    void execute() override;
public:
    explicit ResetButtonTask();
};


#endif //AIRQUALITY_RESETBUTTONTASK_H
