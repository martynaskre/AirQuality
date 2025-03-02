//
// Created by Martynas Skrebė on 17/01/2025.
//

#ifndef AIRQUALITY_WEBSOCKETSERVER_H
#define AIRQUALITY_WEBSOCKETSERVER_H


#include <vector>
#include "mongoose.h"

class WebSocketServer {
    typedef void (*SenderHandler)(mg_mgr *manager, void *data);

    struct Sender {
        SenderHandler handler;
        TickType_t sendEvery;
        void *data;
        TickType_t lastSentAt;
    };

    mg_mgr manager;
    std::vector<Sender> senders;

    static void requestHandler(struct mg_connection *connection, int event, void *eventData);
public:
    WebSocketServer();
    ~WebSocketServer();

    void start();

    bool hasActiveConnections() const;

    void addSender(SenderHandler handler, TickType_t sendEvery, void *data = nullptr);
};


#endif //AIRQUALITY_WEBSOCKETSERVER_H
