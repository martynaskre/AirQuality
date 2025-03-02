//
// Created by Martynas Skrebė on 01/12/2024.
//

#ifndef AIRQUALITY_PAIRINGWEBSERVERTASK_H
#define AIRQUALITY_PAIRINGWEBSERVERTASK_H


#include "Tasks/FreeRTOSTask.h"
#include "mongoose.h"
#include "Network/HTTPServer.h"

class PairingWebServerTask: public FreeRTOSTask {
    HTTPServer httpServer;
    bool credentialsEntered;

    static void handleSensorsEndpoint(mg_connection *connection, mg_http_message *httpMessage, void *data);
    static void handleWifiEndpoint(mg_connection *connection, mg_http_message *httpMessage, void *data);
protected:
    void execute() override;
public:
    explicit PairingWebServerTask();
};


#endif //AIRQUALITY_PAIRINGWEBSERVERTASK_H
