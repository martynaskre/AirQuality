//
// Created by Martynas Skrebė on 12/01/2025.
//

#ifndef AIRQUALITY_SENSORUPDATETASK_H
#define AIRQUALITY_SENSORUPDATETASK_H


#include "Tasks/FreeRTOSTask.h"

class SensorUpdateTask: public FreeRTOSTask {
protected:
    void execute() override;
public:
    explicit SensorUpdateTask();
};


#endif //AIRQUALITY_SENSORUPDATETASK_H
