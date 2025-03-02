//
// Created by Martynas Skrebė on 28/11/2024.
//

#ifndef AIRQUALITY_NETWORK_H
#define AIRQUALITY_NETWORK_H


#include "DHCPServer.h"
#include "lwip/apps/mdns.h"
#include <string>
#include <optional>

enum NetworkMode {
    None,
    AccessPoint,
    Station
};

class Network {
    static NetworkMode mode;
    static std::optional<DHCPServer> dhcpServer;
    static bool mdnsInitialized;

    static void initMdns();
public:
    static err_t createAccessPoint();
    static err_t closeAccessPoint();
    static err_t joinNetwork(const std::string& ssid, const std::string& password, uint64_t timeout = 5000);
    static err_t testNetwork(const std::string& ssid, const std::string& password, uint64_t timeout = 5000);
    static err_t closeStation();
};


#endif //AIRQUALITY_NETWORK_H
