//
// Created by Martynas Skrebė on 01/12/2024.
//

#ifndef AIRQUALITY_HTTPSERVER_H
#define AIRQUALITY_HTTPSERVER_H


#include "mongoose.h"
#include <string>
#include <vector>

class HTTPServer {
    typedef void (*EndpointHandler)(mg_connection *connection, mg_http_message *httpMessage, void *data);

    struct HttpEndpoint {
        std::string method;
        std::string path;
        EndpointHandler handler;
        void *data;
    };

    mg_mgr manager;
    std::vector<HttpEndpoint> endpoints;

    static void requestHandler(struct mg_connection *connection, int event, void *eventData);
public:
    HTTPServer();
    ~HTTPServer();

    void start();
    void startListen();
    void update();

    void get(const std::string &path, EndpointHandler handler, void *data = nullptr);
    void post(const std::string &path, EndpointHandler handler, void *data = nullptr);
};


#endif //AIRQUALITY_HTTPSERVER_H
