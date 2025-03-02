//
// Created by Martynas Skrebė on 17/01/2025.
//

#ifndef AIRQUALITY_WEBSOCKETTASK_H
#define AIRQUALITY_WEBSOCKETTASK_H


#include "Tasks/FreeRTOSTask.h"
#include "Network/WebSocketServer.h"

class WebSocketTask: public FreeRTOSTask {
    WebSocketServer server;

    static void sendSensorData(mg_mgr *manager, void *data);
protected:
    void execute() override;
public:
    explicit WebSocketTask();
};


#endif //AIRQUALITY_WEBSOCKETTASK_H
