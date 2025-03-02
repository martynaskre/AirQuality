//
// Created by Martynas Skrebė on 17/01/2025.
//

#ifndef AIRQUALITY_HTTPCLIENT_H
#define AIRQUALITY_HTTPCLIENT_H


#include <string>

class HTTPClient {
    enum HTTPRequestType {
        GET,
        POST,
        DELETE
    };

    struct HTTPRequest {
        HTTPRequestType type;
        std::string url;
        std::string body;
        int timeout;
        std::string response;
        bool finished = false;
    };

    static std::string urlToHost(std::string url);
    static std::string formatHttpRequest(const HTTPRequest& request);
    static void requestHandler(struct mg_connection *connection, int event, void *eventData);
    static HTTPRequest request(HTTPRequest request);
public:
    static std::string get(const std::string& url, int timeout = 1000);
    static std::string post(const std::string& url, const std::string& body, int timeout = 1000);
    static std::string destroy(const std::string& url, int timeout = 1000);
};


#endif //AIRQUALITY_HTTPCLIENT_H
