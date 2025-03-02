//
// Created by Martynas Skrebė on 28/11/2024.
// Adapted from https://github.com/micropython/micropython/blob/master/shared/netutils/dhcpserver.c
//

#include "DHCPServer.h"

#include <cstdio>
#include <cstring>
#include <cerrno>
#include "Utils/Utils.h"

#include "cyw43_config.h"
#include "lwip/udp.h"

DHCPServer::DHCPServer(ip_addr_t ip, ip_addr_t networkMask): ip(ip), networkMask(networkMask), lease{}, udp(nullptr) {
    memset(lease, 0, sizeof(lease));

    if (DHCPServer::socketNewDgram(&this->udp, this, DHCPServer::serverProcess) != 0) {
        return;
    }

    DHCPServer::socketBind(&this->udp);
}

DHCPServer::~DHCPServer() {
    if (udp != nullptr) {
        udp_remove(udp);

        udp = nullptr;
    }
}

int DHCPServer::socketNewDgram(struct udp_pcb **udp, void *cb_data, udp_recv_fn cb_udp_recv) {
    *udp = udp_new();

    if (*udp == nullptr) {
        return -ENOMEM;
    }

    udp_recv(*udp, cb_udp_recv, (void *)cb_data);

    return ERR_OK;
}

int DHCPServer::socketBind(struct udp_pcb **udp) {
    return udp_bind(*udp, IP_ANY_TYPE, PORT_DHCP_SERVER);
}

void DHCPServer::serverProcess(void *arg, struct udp_pcb *upcb, struct pbuf *p, const ip_addr_t *src_addr, u16_t src_port) {
    auto* server = static_cast<DHCPServer*>(arg);

    (void)upcb;
    (void)src_addr;
    (void)src_port;

    // This is around 548 bytes
    DHCPMessage dhcpMessage;

    if (p->tot_len < DHCP_MIN_SIZE) {
        pbuf_free(p);

        return;
    }

    size_t len = pbuf_copy_partial(p, &dhcpMessage, sizeof(dhcpMessage), 0);
    if (len < DHCP_MIN_SIZE) {
        pbuf_free(p);

        return;
    }

    dhcpMessage.op = DHCPOFFER;
    memcpy(&dhcpMessage.yiaddr, &ip4_addr_get_u32(ip_2_ip4(&server->ip)), 4);

    auto *opt = (uint8_t *) &dhcpMessage.options;
    opt += 4; // assume magic cookie: 99, 130, 83, 99

    uint8_t *msgtype = DHCPServer::optFind(opt, DHCP_OPT_MSG_TYPE);

    if (msgtype == nullptr) {
        pbuf_free(p);

        return;
    }

    switch (msgtype[2]) {
        case DHCPDISCOVER: {
            int yi = DHCPS_MAX_IP;
            for (int i = 0; i < DHCPS_MAX_IP; ++i) {
                if (memcmp(server->lease[i].mac, dhcpMessage.chaddr, MAC_LEN) == 0) {
                    // MAC match, use this IP address
                    yi = i;
                    break;
                }
                if (yi == DHCPS_MAX_IP) {
                    // Look for a free IP address
                    if (memcmp(server->lease[i].mac, "\x00\x00\x00\x00\x00\x00", MAC_LEN) == 0) {
                        // IP available
                        yi = i;
                    }
                    uint32_t expiry = server->lease[i].expiry << 16 | 0xffff;
                    if ((int32_t)(expiry - cyw43_hal_ticks_ms()) < 0) {
                        // IP expired, reuse it
                        memset(server->lease[i].mac, 0, MAC_LEN);
                        yi = i;
                    }
                }
            }
            if (yi == DHCPS_MAX_IP) {
                pbuf_free(p);

                return;
            }
            dhcpMessage.yiaddr[3] = DHCPS_BASE_IP + yi;
            DHCPServer::optWriteU8(&opt, DHCP_OPT_MSG_TYPE, DHCPOFFER);
            break;
        }

        case DHCPREQUEST: {
            uint8_t *o = DHCPServer::optFind(opt, DHCP_OPT_REQUESTED_IP);
            if (o == nullptr) {
                // Should be NACK
                pbuf_free(p);

                return;
            }
            if (memcmp(o + 2, &ip4_addr_get_u32(ip_2_ip4(&server->ip)), 3) != 0) {
                // Should be NACK
                pbuf_free(p);

                return;
            }
            uint8_t yi = o[5] - DHCPS_BASE_IP;
            if (yi >= DHCPS_MAX_IP) {
                // Should be NACK
                pbuf_free(p);

                return;
            }
            if (memcmp(server->lease[yi].mac, dhcpMessage.chaddr, MAC_LEN) == 0) {
                // MAC match, ok to use this IP address
            } else if (memcmp(server->lease[yi].mac, "\x00\x00\x00\x00\x00\x00", MAC_LEN) == 0) {
                // IP unused, ok to use this IP address
                memcpy(server->lease[yi].mac, dhcpMessage.chaddr, MAC_LEN);
            } else {
                // IP already in use
                // Should be NACK
                pbuf_free(p);

                return;
            }
            server->lease[yi].expiry = (cyw43_hal_ticks_ms() + DEFAULT_LEASE_TIME_S * 1000) >> 16;
            dhcpMessage.yiaddr[3] = DHCPS_BASE_IP + yi;
            DHCPServer::optWriteU8(&opt, DHCP_OPT_MSG_TYPE, DHCPACK);
            debug("DHCPS: client connected: MAC=%02x:%02x:%02x:%02x:%02x:%02x IP=%u.%u.%u.%u\n",
                   dhcpMessage.chaddr[0], dhcpMessage.chaddr[1], dhcpMessage.chaddr[2], dhcpMessage.chaddr[3], dhcpMessage.chaddr[4], dhcpMessage.chaddr[5],
                   dhcpMessage.yiaddr[0], dhcpMessage.yiaddr[1], dhcpMessage.yiaddr[2], dhcpMessage.yiaddr[3]);
            break;
        }

        default:
            pbuf_free(p);

            return;
    }

    DHCPServer::optWrite(&opt, DHCP_OPT_SERVER_ID, 4, &ip4_addr_get_u32(ip_2_ip4(&server->ip)));
    DHCPServer::optWrite(&opt, DHCP_OPT_SUBNET_MASK, 4, &ip4_addr_get_u32(ip_2_ip4(&server->networkMask)));
    DHCPServer::optWrite(&opt, DHCP_OPT_ROUTER, 4, &ip4_addr_get_u32(ip_2_ip4(&server->ip))); // aka gateway; can have multiple addresses
    DHCPServer::optWrite(&opt, DHCP_OPT_DNS, 4, &ip4_addr_get_u32(ip_2_ip4(&server->ip))); // this server is the dns
    DHCPServer::optWriteU32(&opt, DHCP_OPT_IP_LEASE_TIME, DEFAULT_LEASE_TIME_S);
    *opt++ = DHCP_OPT_END;
    struct netif *nif = ip_current_input_netif();

    DHCPServer::socketSendto(&server->udp, nif, &dhcpMessage, opt - (uint8_t *)&dhcpMessage, 0xffffffff, PORT_DHCP_CLIENT);

    pbuf_free(p);
}

int DHCPServer::socketSendto(struct udp_pcb **udp, struct netif *nif, const void *buf, size_t len, uint32_t ip, uint16_t port) {
    if (len > 0xffff) {
        len = 0xffff;
    }

    struct pbuf *p = pbuf_alloc(PBUF_TRANSPORT, len, PBUF_RAM);

    if (p == nullptr) {
        return -ENOMEM;
    }

    memcpy(p->payload, buf, len);

    ip_addr_t dest;
    IP4_ADDR(ip_2_ip4(&dest), ip >> 24 & 0xff, ip >> 16 & 0xff, ip >> 8 & 0xff, ip & 0xff);
    err_t err;

    if (nif != nullptr) {
        err = udp_sendto_if(*udp, p, &dest, port, nif);
    } else {
        err = udp_sendto(*udp, p, &dest, port);
    }

    pbuf_free(p);

    if (err != ERR_OK) {
        return err;
    }

    return len;
}

uint8_t *DHCPServer::optFind(uint8_t *opt, uint8_t cmd) {
    for (int i = 0; i < 308 && opt[i] != DHCP_OPT_END;) {
        if (opt[i] == cmd) {
            return &opt[i];
        }

        i += 2 + opt[i + 1];
    }

    return nullptr;
}

void DHCPServer::optWrite(uint8_t **opt, uint8_t cmd, size_t n, const void *data) {
    uint8_t *o = *opt;
    *o++ = cmd;
    *o++ = n;
    memcpy(o, data, n);
    *opt = o + n;
}

void DHCPServer::optWriteU8(uint8_t **opt, uint8_t cmd, uint8_t val) {
    uint8_t *o = *opt;
    *o++ = cmd;
    *o++ = 1;
    *o++ = val;
    *opt = o;
}

void DHCPServer::optWriteU32(uint8_t **opt, uint8_t cmd, uint32_t val) {
    uint8_t *o = *opt;
    *o++ = cmd;
    *o++ = 4;
    *o++ = val >> 24;
    *o++ = val >> 16;
    *o++ = val >> 8;
    *o++ = val;
    *opt = o;
}