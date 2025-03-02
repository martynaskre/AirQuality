//
// Created by Martynas Skrebė on 29/01/2025.
//

#ifndef AIRQUALITY_TIMEDCOMMANDTASK_H
#define AIRQUALITY_TIMEDCOMMANDTASK_H

#include "Tasks/FreeRTOSTask.h"
#include "Tasks/ManagerTask.h"

class TimedCommandTask: public FreeRTOSTask {
    ManagerCommand command;
    uint64_t delay;
protected:
    void execute() override;
public:
    explicit TimedCommandTask(ManagerCommand command, uint64_t delay);
};


#endif //AIRQUALITY_TIMEDCOMMANDTASK_H
