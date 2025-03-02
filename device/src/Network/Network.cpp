//
// Created by Martynas Skrebė on 28/11/2024.
//

#include "Network.h"
#include "pico/cyw43_arch.h"
#include "Utils/Utils.h"

NetworkMode Network::mode = None;
std::optional<DHCPServer> Network::dhcpServer = std::nullopt;
bool Network::mdnsInitialized = false;

void Network::initMdns() {
    if (!Network::mdnsInitialized) {
        mdns_resp_init();

        Network::mdnsInitialized = true;
    }
}

err_t Network::createAccessPoint() {
    if (!initialiseNetworkDriver()) {
        return ERR_CONN;
    }

    closeAccessPoint();
    closeStation();

    std::string ssid = "AirQuality";

    cyw43_arch_enable_ap_mode(ssid.c_str(), nullptr, CYW43_AUTH_WPA2_AES_PSK);
    Network::mode = AccessPoint;

    ip4_addr_t ip;
    ip4_addr_t mask;

    IP4_ADDR(ip_2_ip4(&ip), 192, 168, 4, 1);
    IP4_ADDR(ip_2_ip4(&mask), 255, 255, 255, 0);

    dhcpServer.emplace(ip, mask);

    initMdns();
    mdns_resp_add_netif(&cyw43_state.netif[CYW43_ITF_AP], "pairing");

    return ERR_OK;
}

err_t Network::closeAccessPoint() {
    if (!initialiseNetworkDriver()) {
        return ERR_CONN;
    }

    if (Network::mode == AccessPoint) {
        dhcpServer.reset();

        if (Network::mdnsInitialized && mdns_resp_netif_active(&cyw43_state.netif[CYW43_ITF_AP]) != 0) {
            auto mDNS_error = mdns_resp_remove_netif(&cyw43_state.netif[CYW43_ITF_AP]);

            if (mDNS_error != ERR_OK) {
                return mDNS_error;
            }
        }

        cyw43_arch_disable_ap_mode();

        Network::mode = None;

        return ERR_OK;
    }

    return ERR_CONN;
}

err_t Network::joinNetwork(const std::string& ssid, const std::string& password, uint64_t delay) {
    if (!initialiseNetworkDriver()) {
        return ERR_CONN;
    }

    closeStation();
    closeAccessPoint();

    cyw43_arch_enable_sta_mode();
    Network::mode = Station;

    if (cyw43_arch_wifi_connect_timeout_ms(ssid.c_str(), password.c_str(), CYW43_AUTH_WPA2_AES_PSK, delay)) {
        return ERR_TIMEOUT;
    }

    initMdns();
    mdns_resp_add_netif(&cyw43_state.netif[CYW43_ITF_STA], "AirQuality");

    return ERR_OK;
}

err_t Network::testNetwork(const std::string &ssid, const std::string &password, uint64_t timeout) {
    auto joinResponse = Network::joinNetwork(ssid, password, timeout);

    Network::closeStation();

    return joinResponse;
}

err_t Network::closeStation() {
    if (!initialiseNetworkDriver()) {
        return ERR_CONN;
    }

    if (Network::mode == Station) {
        if (Network::mdnsInitialized && mdns_resp_netif_active(&cyw43_state.netif[CYW43_ITF_STA]) != 0) {
            auto mDNS_error = mdns_resp_remove_netif(&cyw43_state.netif[CYW43_ITF_STA]);

            if (mDNS_error != ERR_OK) {
                return mDNS_error;
            }
        }

        cyw43_arch_disable_sta_mode();

        return ERR_OK;
    }

    return ERR_CONN;
}
