#ifndef HPC_NET_H
#define HPC_NET_H

//#define DEBUG
#include "../../HPC_Integral/lib/debug_func.h"
#include <stdint.h>
#include <sys/socket.h>

#define SA struct sockaddr

#define CHECK_ERR(act, err_act)     \
    if ((act) < 0) {                \
        PRINT_ERROR (#act);         \
        do { err_act; } while (0);  \
        return -1;                  \
    }

const char KEY_START_SERVER[] = "server";
const uint16_t SERV_PORT_MIN = 1025;
const uint16_t SERV_PORT_MAX = 64000;

const uint16_t UDP_BROADCAST_PORT = 8000;

const char LAN_IP_SERVER[] = "192.168.0.1";

int addrGetPort (const void* paddr);

int hpcnetBroadcastServerInit (struct sockaddr_in* pservaddr);
int hpcnetBroadcastClientInit (struct sockaddr_in* pservaddr);
int hpcnetTcpServerInit (char* host, int clients_limit, struct sockaddr_in* pservaddr);
int hpcnetTcpLanClientInit (uint16_t tcpserv_port);

int GetStaticIP (char* if_name, sa_family_t family, char host[]);

#endif // HPC_NET_H