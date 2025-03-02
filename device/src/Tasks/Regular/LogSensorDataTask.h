//
// Created by Martynas Skrebė on 17/01/2025.
//

#ifndef AIRQUALITY_LOGSENSORDATATASK_H
#define AIRQUALITY_LOGSENSORDATATASK_H


#include "Tasks/FreeRTOSTask.h"

class LogSensorDataTask: public FreeRTOSTask {
protected:
    void execute() override;
public:
    explicit LogSensorDataTask();
};


#endif //AIRQUALITY_LOGSENSORDATATASK_H
