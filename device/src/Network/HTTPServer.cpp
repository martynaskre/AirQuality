//
// Created by Martynas Skrebė on 01/12/2024.
//

#include "HTTPServer.h"

HTTPServer::HTTPServer() {
    mg_mgr_init(&manager);
}

HTTPServer::~HTTPServer() {
    mg_mgr_free(&manager);
}

void HTTPServer::requestHandler(struct mg_connection *connection, int event, void *eventData) {
    if (event == MG_EV_HTTP_MSG) {
        auto* httpServer = static_cast<HTTPServer*>(connection->fn_data);
        auto* httpMessage = static_cast<mg_http_message*>(eventData);

        for (auto &endpoint: httpServer->endpoints) {
            if (
                mg_match(httpMessage->uri, mg_str(endpoint.path.c_str()), nullptr) &&
                mg_match(httpMessage->method, mg_str(endpoint.method.c_str()), nullptr)
            ) {
                endpoint.handler(connection, httpMessage, endpoint.data);
            }
        }

        mg_http_reply(connection, 404, "Content-Type: application/json\r\n",
                      "{%m:%m}\n", MG_ESC("status"), MG_ESC("not_found"));
    }
}

void HTTPServer::start() {
    startListen();

    while (true) {
        update();
    }
}

void HTTPServer::startListen() {
    mg_http_listen(&manager, "http://0.0.0.0:80", HTTPServer::requestHandler, this);
}

void HTTPServer::update() {
    mg_mgr_poll(&manager, 10);
}

void HTTPServer::get(const std::string &path, HTTPServer::EndpointHandler handler, void *data) {
    endpoints.push_back({ "GET", path, handler, data });
}

void HTTPServer::post(const std::string &path, HTTPServer::EndpointHandler handler, void *data) {
    endpoints.push_back({ "POST", path, handler, data });
}
