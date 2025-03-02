//
// Created by Martynas Skrebė on 17/01/2025.
//

#ifndef AIRQUALITY_MANAGERTASK_H
#define AIRQUALITY_MANAGERTASK_H


#include "FreeRTOSTask.h"
#include "Tasks/Utils/ResetButtonTask.h"
#include "Tasks/Utils/ResourcesTask.h"
#include <vector>
#include <memory>
#include <optional>

enum ManagerCommand {
    START_PAIRING = 1,
    VALIDATE_NETWORK = 2,
    START_REGULAR = 3,
    RESET = 4,
};

class ManagerTask: public FreeRTOSTask {
    static ManagerTask* instance;

    std::optional<ManagerCommand> currentCommand;

    ResetButtonTask resetButtonTask;
    ResourcesTask resourcesTask;
    std::vector<std::unique_ptr<FreeRTOSTask>> tasks;

    explicit ManagerTask();
    ~ManagerTask() override;

    void reset(bool hard = false);

    void startPairing();
    void startNetworkValidation();
    void startRegular();
public:
    ManagerTask(const ManagerTask&) = delete;
    ManagerTask& operator=(const ManagerTask&) = delete;

    std::optional<ManagerCommand>& getCurrentCommand();

    static ManagerTask* getInstance();
    static void notify(ManagerCommand command);
protected:
    void execute() override;
};


#endif //AIRQUALITY_MANAGERTASK_H
