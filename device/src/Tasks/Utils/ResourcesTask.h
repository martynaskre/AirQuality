//
// Created by Martynas Skrebė on 22/02/2025.
//

#ifndef AIRQUALITY_RESOURCESTASK_H
#define AIRQUALITY_RESOURCESTASK_H


#include "Tasks/FreeRTOSTask.h"

class ResourcesTask: public FreeRTOSTask {
protected:
    void execute() override;
public:
    explicit ResourcesTask();
};


#endif //AIRQUALITY_RESOURCESTASK_H
