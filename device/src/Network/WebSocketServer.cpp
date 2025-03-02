//
// Created by Martynas Skrebė on 17/01/2025.
//

#include "WebSocketServer.h"

WebSocketServer::WebSocketServer() {
    mg_mgr_init(&manager);
}

WebSocketServer::~WebSocketServer() {
    mg_mgr_free(&manager);
}

void WebSocketServer::requestHandler(struct mg_connection *connection, int event, void *eventData) {
    if (event == MG_EV_HTTP_MSG) {
        auto* httpMessage = static_cast<mg_http_message*>(eventData);

        if (mg_match(httpMessage->uri, mg_str("/ws"), NULL)) {
            mg_ws_upgrade(connection, httpMessage, NULL);

            connection->data[0] = 'W';
        } else {
            mg_http_reply(connection, 404, "Content-Type: application/json\r\n",
                          "{%m:%m}\n", MG_ESC("status"), MG_ESC("not_found"));
        }
    }
}

void WebSocketServer::start() {
    mg_http_listen(&manager, "http://0.0.0.0:80", WebSocketServer::requestHandler, nullptr);

    while (true) {
        mg_mgr_poll(&manager, 10);

        if (hasActiveConnections()) {
            TickType_t currentTick = xTaskGetTickCount();

            for (auto &sender: senders) {
                if (currentTick - sender.lastSentAt >= pdMS_TO_TICKS(sender.sendEvery)) {
                    sender.handler(&manager, sender.data);
                    sender.lastSentAt = currentTick;
                }
            }
        }
    }
}

void WebSocketServer::addSender(WebSocketServer::SenderHandler handler, TickType_t sendEvery, void *data) {
    senders.push_back({ handler, sendEvery, data, xTaskGetTickCount() });
}

bool WebSocketServer::hasActiveConnections() const {
    for (mg_connection *connection = manager.conns; connection != nullptr; connection = connection->next) {
        if (connection->data[0] == 'W') {
            return true;
        }
    }

    return false;
}
