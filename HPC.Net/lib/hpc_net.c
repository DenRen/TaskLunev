#include "hpc_net.h"

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
#include <signal.h>

// ===============\\
// Other functions -------------------------------------------------------------
// ===============//

int getAddr (struct sockaddr_in* paddr) {
    char host_str[NI_MAXHOST], serv_str[NI_MAXSERV];
    CHECK_ERR (
        getnameinfo ((SA*) paddr, sizeof (*paddr),
                     host_str, sizeof (host_str), serv_str, sizeof (serv_str),
                     NI_NUMERICHOST | NI_NUMERICSERV),
    );

    printf ("host: %s\nserv: %s\n", host_str, serv_str);

    return 0;
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

// ===========================\\
// UDP Client/Server functions -------------------------------------------------------------
// ===========================//

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

int SendDataBroadcast (void *data, size_t len) {
    
    // Create broadcast address
    struct sockaddr_in addr = {0};
    bzero (&addr, sizeof (addr));

    addr.sin_family = AF_INET;
    addr.sin_port = htons (UDP_BROADCAST_PORT);
    addr.sin_addr.s_addr = htonl (INADDR_BROADCAST);

    // Create socket
    int sockfd = socket (AF_INET, SOCK_DGRAM, 0);
    CHECK_ERR (sockfd, );

    // Enable on socket broadcast and send datagram (data)
    int enable = 1;
    CHECK_ERR (setsockopt (sockfd, SOL_SOCKET, SO_BROADCAST, &enable, sizeof (enable)), close (sockfd));
    CHECK_ERR (sendto (sockfd, data, len, 0, (SA*) &addr, sizeof (addr)),               close (sockfd));

    return 0;
}

int RecvDataBroadcast (void* buf, size_t len) {
    struct sockaddr_in servaddr = {0};
    bzero (&servaddr, sizeof (servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons (UDP_BROADCAST_PORT);
    servaddr.sin_addr.s_addr = htonl (INADDR_ANY);

    int sockfd = socket (AF_INET, SOCK_DGRAM, 0);
    CHECK_ERR (sockfd, );

    CHECK_ERR (bind (sockfd, (SA*) &servaddr, sizeof (servaddr)), close (sockfd));
    CHECK_ERR (recvfrom (sockfd, buf, len, 0, NULL, NULL),        close (sockfd));

    return 0;
}

// ===========================\\
// TCP Client/Server functions -------------------------------------------------------------
// ===========================//

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

int hpcnetTcpClientInit (const struct sockaddr_in* servaddr) {
    
    int clifd = socket (AF_INET, SOCK_STREAM, 0);
    CHECK_ERR (clifd, );

    struct sockaddr_in clean_addr = {0};
    bzero (&clean_addr, sizeof (clean_addr));

    clean_addr.sin_family = AF_INET;
    clean_addr.sin_addr = servaddr->sin_addr;
    clean_addr.sin_port = servaddr->sin_port;

    CHECK_ERR (connect (clifd, (SA*) &clean_addr, sizeof (clean_addr)), close (clifd));
    
    return clifd;
}

// ================\\
// System functions -------------------------------------------------------------
// ================//

// Only for call EINT from accept
static void callback_alarm (int signum) {
    return;
}

// Only for AF_INET and AF_INET6
int GetStaticAddr (const char* if_name, sa_family_t family, char str_host[], uint32_t len_host) {
    struct ifaddrs* ifaddr = NULL;
    CHECK_ERR (getifaddrs (&ifaddr), );

    for (struct ifaddrs* ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL                ||
            strcmp (ifa->ifa_name, if_name) != 0 ||
            ifa->ifa_addr->sa_family != family)
        {
            continue;
        }

        // Get strong host
        CHECK_ERR (
            getnameinfo (ifa->ifa_addr,
                         (family == AF_INET) ? sizeof (struct sockaddr_in) :
                                               sizeof (struct sockaddr_in6),
                         str_host, len_host,
                         NULL, 0, NI_NUMERICHOST),
        );

        return 0;
    }

    errno = EINVAL;
    return -1;
}

// ===============\\
// Start functions -------------------------------------------------------------
// ===============//

int StartClient () {
    // Получаем адрес сервера (servaddr) из широковещательного канала (UDP BC)
    struct sockaddr_in servaddr = {0};
    CHECK_ERR (RecvDataBroadcast ((void*) &servaddr, sizeof (servaddr)), );

    IF_DEBUG (
        char buf[INET_ADDRSTRLEN] = "";
        CHECK_ERR (inet_ntop (AF_INET, &servaddr.sin_addr, buf, sizeof (buf)), );
        printf ("Server address: %s:%d\n", buf, ntohs (servaddr.sin_port));
    );
    
    // Connect at the servaddr to the TCP server 
    int tcpcli_fd = hpcnetTcpClientInit (&servaddr);
    CHECK_ERR (tcpcli_fd, );
    printf ("Client HPC Net connected to TCP server\n");

    #define ERR_ACT close (tcpcli_fd)

    // ...

    #undef ERR_ACT
    close (tcpcli_fd);

    printf ("Client HPC Net correct finished\n");

    return 0;
}

int StartServer (const int clients_limit, const char* if_name, sa_family_t family) {
    
    if (clients_limit <= 0 || if_name == NULL) {
        PRINT_ERROR ("clients_limit <= 0 || if_name == NULL");
        return -1;
    }

    if (family != AF_INET) { // Add && family != AF_INET6
        char errstr[MAXLEN_ERRSTR] = {0}; 
        snprintf (errstr, sizeof (errstr), "Socket family %d is not supported!\n", family);
        PRINT_ERROR (errstr);
        return -1;
    }

    // Get static IP from if_name and sa_family
    char host[NI_MAXHOST] = {0};
    CHECK_ERR (GetStaticAddr (if_name, family, host, sizeof (host)), );

    IF_DEBUG (printf ("Getted static host server: %s\n", host));

    // Start TCP server
    struct sockaddr_in tcpserv_addr = {0};
    int tcpserv_fd = hpcnetTcpServerInit (host, clients_limit, &tcpserv_addr);
    CHECK_ERR (tcpserv_fd, );
    printf ("TCP server HPC Net started\n");

    #define ERR_ACT close (tcpserv_fd)

    IF_DEBUG (printf ("TCP server address: %s:%d\n", host, ntohs (addrGetPort (&tcpserv_addr))));

    // Send server static address to broadcast
    CHECK_ERR (SendDataBroadcast (&tcpserv_addr, sizeof (tcpserv_addr)), ERR_ACT);

    // Block all signals except SIGALARM for next step
    sigset_t sigset_noalarm = {0};
    sigfillset (&sigset_noalarm);
    sigdelset  (&sigset_noalarm, SIGALRM);

    struct sigaction alaram_act = {0};
    bzero (&alaram_act, sizeof (alaram_act));
    alaram_act.sa_handler = callback_alarm;
    sigfillset (&alaram_act.sa_mask);

    CHECK_ERR (sigaction (SIGALRM, &alaram_act, NULL), ERR_ACT);
    CHECK_ERR (sigprocmask (SIG_BLOCK, &sigset_noalarm, NULL), ERR_ACT);

    // Accept of clients
    alarm (5);  // Max time wait loop of accept

    int clifds[clients_limit];
    int numcli = 0;
    for (; numcli < clients_limit; ++numcli)
        if ((clifds[numcli] = accept (tcpserv_fd, NULL, NULL)) == -1) {
            if (errno == EINTR)
                break;
            else {
                CHECK_ERR (-1, ERR_ACT);
            }
        }

    alarm (0);
    
    printf ("Number clients: %d\n", numcli);

    for (int i = 0; i < numcli; ++i) {
        close (clifds[i]);  // todo: Check on block!!!
    }

    #undef ERR_ACT
    close (tcpserv_fd);

    printf ("Server HPC Net correct finished\n");
    return 0;
}