//
// Created by Martynas Skrebė on 28/11/2024.
// Adapted from https://github.com/micropython/micropython/blob/master/shared/netutils/dhcpserver.h
//

#ifndef AIRQUALITY_DHCPSERVER_H
#define AIRQUALITY_DHCPSERVER_H


#include "lwip/ip_addr.h"
#include "lwip/udp.h"

#define DHCPS_BASE_IP (16)
#define DHCPS_MAX_IP (8)

#define PORT_DHCP_SERVER (67)
#define PORT_DHCP_CLIENT (68)

#define DEFAULT_LEASE_TIME_S (24 * 60 * 60) // in seconds

#define DHCPDISCOVER    (1)
#define DHCPOFFER       (2)
#define DHCPREQUEST     (3)
#define DHCPDECLINE     (4)
#define DHCPACK         (5)
#define DHCPNACK        (6)
#define DHCPRELEASE     (7)
#define DHCPINFORM      (8)

#define DHCP_MIN_SIZE (240 + 3)

#define DHCP_OPT_PAD                (0)
#define DHCP_OPT_SUBNET_MASK        (1)
#define DHCP_OPT_ROUTER             (3)
#define DHCP_OPT_DNS                (6)
#define DHCP_OPT_HOST_NAME          (12)
#define DHCP_OPT_REQUESTED_IP       (50)
#define DHCP_OPT_IP_LEASE_TIME      (51)
#define DHCP_OPT_MSG_TYPE           (53)
#define DHCP_OPT_SERVER_ID          (54)
#define DHCP_OPT_PARAM_REQUEST_LIST (55)
#define DHCP_OPT_MAX_MSG_SIZE       (57)
#define DHCP_OPT_VENDOR_CLASS_ID    (60)
#define DHCP_OPT_CLIENT_ID          (61)
#define DHCP_OPT_END                (255)

#define MAC_LEN (6)

struct DHCPLease {
    uint8_t mac[MAC_LEN];
    uint16_t expiry;
};

struct DHCPMessage {
    uint8_t op; // message opcode
    uint8_t htype; // hardware address type
    uint8_t hlen; // hardware address length
    uint8_t hops;
    uint32_t xid; // transaction id, chosen by client
    uint16_t secs; // client seconds elapsed
    uint16_t flags;
    uint8_t ciaddr[4]; // client IP address
    uint8_t yiaddr[4]; // your IP address
    uint8_t siaddr[4]; // next server IP address
    uint8_t giaddr[4]; // relay agent IP address
    uint8_t chaddr[16]; // client hardware address
    uint8_t sname[64]; // server host name
    uint8_t file[128]; // boot file name
    uint8_t options[312]; // optional parameters, variable, starts with magic
};

class DHCPServer {
    ip_addr_t ip;
    ip_addr_t networkMask;
    DHCPLease lease[DHCPS_MAX_IP];
    struct udp_pcb *udp;

    static int socketNewDgram(struct udp_pcb **udp, void *cb_data, udp_recv_fn cb_udp_recv);
    static int socketBind(struct udp_pcb **udp);
    static void serverProcess(void *arg, struct udp_pcb *upcb, struct pbuf *p, const ip_addr_t *src_addr, u16_t src_port);
    static int socketSendto(struct udp_pcb **udp, struct netif *nif, const void *buf, size_t len, uint32_t ip, uint16_t port);

    static uint8_t *optFind(uint8_t *opt, uint8_t cmd);
    static void optWrite(uint8_t **opt, uint8_t cmd, size_t n, const void *data);
    static void optWriteU8(uint8_t **opt, uint8_t cmd, uint8_t val);
    static void optWriteU32(uint8_t **opt, uint8_t cmd, uint32_t val);

public:
    DHCPServer(ip_addr_t ip, ip_addr_t networkMask);
    ~DHCPServer();
};


#endif //AIRQUALITY_DHCPSERVER_H
