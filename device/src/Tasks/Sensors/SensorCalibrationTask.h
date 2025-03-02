//
// Created by Martynas Skrebė on 12/01/2025.
//

#ifndef AIRQUALITY_SENSORCALIBRATIONTASK_H
#define AIRQUALITY_SENSORCALIBRATIONTASK_H


#include "Tasks/FreeRTOSTask.h"

class SensorCalibrationTask: public FreeRTOSTask {
protected:
    void execute() override;
public:
    explicit SensorCalibrationTask();
};


#endif //AIRQUALITY_SENSORCALIBRATIONTASK_H
