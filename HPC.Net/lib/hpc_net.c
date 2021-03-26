#include "hpc_net.h"

#define DEBUG
#include "../../HPC_Integral/lib/debug_func.h"

#include <netinet/in.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <strings.h>
#include <ifaddrs.h>
#include <netdb.h>

int GetStaticIP (char* if_name, sa_family_t family, char host[]) {
    struct ifaddrs* ifaddr = NULL;
    CHECK_ERR (getifaddrs (&ifaddr), );

    for (struct ifaddrs* ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL                ||
            strcmp (ifa->ifa_name, if_name) != 0 ||
            ifa->ifa_addr->sa_family != family)
        {
            continue;
        }

        CHECK_ERR (
            getnameinfo (ifa->ifa_addr,
                         (family == AF_INET) ? sizeof (struct sockaddr_in) :
                                               sizeof (struct sockaddr_in6),
                         host, NI_MAXHOST,
                         NULL, 0, NI_NUMERICHOST),
        );

        return 0;
    }

    return -1;
}

// In network format. Use ntohs
int addrGetPort (const void* paddr) {
    switch (((struct sockaddr*) paddr)->sa_family) {
        case AF_INET: {
            struct sockaddr_in* paddr_in = (struct sockaddr_in*) paddr;
            return paddr_in->sin_port;
        } break;

        case AF_INET6: {
            struct sockaddr_in6* paddr_in = (struct sockaddr_in6*) paddr;
            return paddr_in->sin6_port;
        } break;
    }

    return -1;
}

static int _broadcastUnitInit (struct sockaddr_in* pservaddr, in_addr_t s_addr) {
    CHECK_PTR (pservaddr);

    // Create socket servfd and set SO_BROADCAST
    int servfd = socket (AF_INET, SOCK_DGRAM, 0);
    CHECK_ERR (servfd, );
    
    #define ERR_ACT close (servfd)

    int enable = 1;
    CHECK_ERR (
        setsockopt (servfd, SOL_SOCKET, SO_BROADCAST, &enable, sizeof (enable)),
        ERR_ACT
    );

    // Init address
    pservaddr->sin_family = AF_INET;
    pservaddr->sin_addr.s_addr = htonl (s_addr);
    pservaddr->sin_port = htons (SERV_PORT_MIN);

    #undef ERR_ACT

    return servfd;
}

int hpcnetBroadcastServerInit (struct sockaddr_in* pservaddr) {
    return _broadcastUnitInit (pservaddr, INADDR_ANY);
}

int hpcnetBroadcastClientInit (struct sockaddr_in* pservaddr) {
    CHECK_PTR (pservaddr);

    // Create socket servfd and set SO_BROADCAST
    int servfd = socket (AF_INET, SOCK_DGRAM, 0);
    CHECK_ERR (servfd, );
    
    #define ERR_ACT close (servfd)
    
    int enable = 1;
    CHECK_ERR (
        setsockopt (servfd, SOL_SOCKET, SO_BROADCAST, &enable, sizeof (enable)),
        ERR_ACT
    );

    // Init address
    pservaddr->sin_family = AF_INET;
    pservaddr->sin_addr.s_addr = htonl (INADDR_ANY);
    pservaddr->sin_port = htons (SERV_PORT_MIN);

    #undef ERR_ACT

    return servfd;
}

// sockfd > 0, paddr != NULL
static int _bindFreePort (int sockfd, struct sockaddr_in* paddr,
                          uint16_t minport, uint16_t maxport) {
    IF_DEBUG (printf ("Start check port from %d to %d\n", minport, maxport));

    int state = 0;
    uint16_t port = minport;
    do {
        IF_DEBUG (printf ("Check port %d\n", port));
        paddr->sin_port = htons (port);
    } while ((state = bind (sockfd, (SA*) paddr, sizeof (*paddr))) == -1 &&
              port++ <= maxport);

    if (port > maxport) {
        IF_DEBUG (printf ("Port not found!\n"));
        return -1;
    }

    IF_DEBUG (printf ("Port %d found and bond with socket %d\n", port, sockfd));

    return port;
}

int hpcnetTcpServerInit (char* host, int clients_limit, struct sockaddr_in* pservaddr) {
    CHECK_PTR (pservaddr);

    // Create socket servfd
    int servfd = socket (AF_INET, SOCK_STREAM, 0);
    CHECK_ERR (servfd, );
    
    #define ERR_ACT close (servfd)

    // Init address
    pservaddr->sin_family = AF_INET;
    CHECK_ERR (inet_pton (AF_INET, host, &pservaddr->sin_addr.s_addr), );

    // Find free port from SERV_PORT_MIN to SERV_PORT_MAX and bind with him
    const int state = _bindFreePort (servfd, pservaddr, SERV_PORT_MIN, SERV_PORT_MAX);
    if (state == -1) {
        printf ("No free ports %d to %d!\n", SERV_PORT_MIN, SERV_PORT_MAX);
        CHECK_ERR (-1, ERR_ACT);
    }

    CHECK_ERR (listen (servfd, clients_limit), ERR_ACT);

    #undef ERR_ACT

    return servfd;
}

int hpcnetTcpLanClientInit (uint16_t tcpserv_port) {
    
    // Init server address
    struct sockaddr_in servaddr = {0};
    bzero (&servaddr, sizeof (servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons (tcpserv_port);
    CHECK_ERR (
        inet_pton (AF_INET, "192.168.43.36", &servaddr.sin_addr.s_addr),
    );

    // Create socket
    int clifd = socket (AF_INET, SOCK_STREAM, 0);
    CHECK_ERR (clifd, );
    
    // Connect ot server
    printf ("Start connecting\n");
    CHECK_ERR (connect (clifd, (SA*) &servaddr, sizeof (servaddr)), close (clifd));
    printf ("Finish connecting\n");

    return clifd;
}