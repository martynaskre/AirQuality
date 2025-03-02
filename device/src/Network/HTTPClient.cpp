//
// Created by Martynas Skrebė on 17/01/2025.
//

#include <sstream>
#include "HTTPClient.h"
#include "mongoose.h"
#include "options.h"

std::string HTTPClient::urlToHost(std::string url) {
    mg_str host = mg_url_host(url.c_str());
    auto hostAsString = std::string(host.buf, host.len);

    size_t pos = hostAsString.find('/');
    return (pos == std::string::npos) ? hostAsString : hostAsString.substr(0, pos);
}

std::string HTTPClient::formatHttpRequest(const HTTPClient::HTTPRequest& request) {
    std::stringstream httpRequest;

    auto host = HTTPClient::urlToHost(request.url);

    switch (request.type) {
        case GET:
            httpRequest << "GET " << mg_url_uri(request.url.c_str()) << " HTTP/1.1\r\n";
            httpRequest << "Host: " << host << "\r\n";
            httpRequest << "Station-Number: " << SERIAL_NUMBER << "\r\n";
            httpRequest << "\r\n";

            break;
        case POST:
            httpRequest << "POST " << mg_url_uri(request.url.c_str()) << " HTTP/1.1\r\n";
            httpRequest << "Host: " << host << "\r\n";
            httpRequest << "Station-Number: " << SERIAL_NUMBER << "\r\n";
            httpRequest << "Push-Key: " << PUSH_KEY << "\r\n";
            httpRequest << "Content-Type: application/json\r\n";
            httpRequest << "Accept: application/json\r\n";
            httpRequest << "Content-Length: " << request.body.length() << "\r\n";
            httpRequest << "\r\n";
            httpRequest << request.body;

            break;
        case DELETE:
            httpRequest << "DELETE " << mg_url_uri(request.url.c_str()) << " HTTP/1.1\r\n";
            httpRequest << "Host: " << host << "\r\n";
            httpRequest << "Station-Number: " << SERIAL_NUMBER << "\r\n";
            httpRequest << "Push-Key: " << PUSH_KEY << "\r\n";
            httpRequest << "Accept: application/json\r\n";
            httpRequest << "Content-Length: " << request.body.length() << "\r\n";
            httpRequest << "\r\n";

            break;
    }

    return httpRequest.str();
}

void HTTPClient::requestHandler(struct mg_connection *connection, int event, void *eventData) {
    auto* request = static_cast<HTTPRequest*>(connection->fn_data);

    if (request == nullptr) {
        return;
    }

    if (event == MG_EV_OPEN) {
        *(uint32_t *) connection->data = xTaskGetTickCount() + pdMS_TO_TICKS(request->timeout);
    } else if (event == MG_EV_POLL) {
        if (xTaskGetTickCount() > *(uint32_t *) connection->data) {
            mg_error(connection, "Connect timeout");
        }
    } else if (event == MG_EV_CONNECT) {
        auto requestBody = HTTPClient::formatHttpRequest(*request);

        mg_send(connection, requestBody.c_str(), requestBody.length());
    } if (event == MG_EV_HTTP_MSG) {
        auto* httpMessage = static_cast<mg_http_message*>(eventData);

        if (httpMessage != nullptr) {
            request->response = std::string(httpMessage->message.buf, httpMessage->message.len);
            request->finished = true;

            connection->is_draining = 1;
        }
    } else if (event == MG_EV_ERROR) {
        request->finished = true;
    }
}

HTTPClient::HTTPRequest HTTPClient::request(HTTPClient::HTTPRequest request) {
    mg_mgr manager;

    mg_mgr_init(&manager);
    mg_http_connect(&manager, request.url.c_str(), HTTPClient::requestHandler, &request);

    while (!request.finished) {
        mg_mgr_poll(&manager, 100);
    }

    mg_mgr_free(&manager);

    return request;
}

std::string HTTPClient::get(const std::string& url, int timeout) {
    auto request = HTTPClient::request({ GET, url, "", timeout, ""});

    return request.response;
}

std::string HTTPClient::post(const std::string& url, const std::string& body, int timeout) {
    auto request = HTTPClient::request({ POST, url, body, timeout, ""});

    return request.response;
}

std::string HTTPClient::destroy(const std::string &url, int timeout) {
    auto request = HTTPClient::request({ DELETE, url, "", timeout, ""});

    return request.response;
}
