#include "learn/unp.h"
#include "lib/hpc_net.h"
#include <netdb.h>
#include <signal.h>

int getAddr (struct sockaddr_in* paddr);

// Only for call EINT from accept
static void callback_alarm (int signum) {
    return;
}

int StartClient ();
int StartServer ();

int main (int argc, char* argv[]) {
    if (argc == 1) {
        CHECK_ERR (StartClient (), );
    }
    else if (argc == 2 && strcmp (argv[1], KEY_START_SERVER) == 0) {
        CHECK_ERR (StartServer (), );   
    }
    else {
        printf ("1) For start client, enter: %s\n"
                "2) For start server, enter: %s server\n", argv[0], argv[0]);
    }
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

int StartClient () {
    struct sockaddr_in servaddr = {0};
    CHECK_ERR (RecvDataBroadcast ((void*) &servaddr, sizeof (servaddr)), );

    char buf[MAX_LINE] = {0};
    CHECK_ERR (inet_ntop (AF_INET, &servaddr.sin_addr, buf, sizeof (buf)), );
    printf ("Server address: %s:%d\n", buf, ntohs (servaddr.sin_port));


    // TODO: HOST + port in next function
    // Create TCP server and connect to LAN_IP_SERVER
    int tcpcli_fd = hpcnetTcpLanClientInit (15);
    CHECK_ERR (tcpcli_fd, );
    printf ("Client HPC Net connected to TCP server\n");

    #define ERR_ACT close (tcpcli_fd)

    // ...

    #undef ERR_ACT
    close (tcpcli_fd);

    printf ("Client HPC Net correct finished\n");

    return 0;
}

int SendDataBroadcast (void *data, size_t len) {
    
    int sockfd = socket (AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in addr = {0};
    
    bzero (&addr, sizeof (addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons (UDP_BROADCAST_PORT);
    addr.sin_addr.s_addr = htonl (INADDR_BROADCAST);

    #define ERR_ACT close (sockfd)

    int enable = 1;
    CHECK_ERR (setsockopt (sockfd, SOL_SOCKET, SO_BROADCAST, &enable, sizeof (enable)), ERR_ACT);
    CHECK_ERR (sendto (sockfd, data, len, 0, (SA*) &addr, sizeof (addr)), ERR_ACT);

    #undef ERR_ACT

    return 0;
}

int StartServer () {

    char host[NI_MAXHOST] = {0};
    CHECK_ERR (GetStaticIP ("wlo1", AF_INET, host), );
    printf ("host: %s\n", host);

    const int clients_limit = 20;

    // Start TCP server
    struct sockaddr_in tcpserv_addr = {0};
    int tcpserv_fd = hpcnetTcpServerInit (host, clients_limit, &tcpserv_addr);
    CHECK_ERR (tcpserv_fd, );
    printf ("TCP server HPC Net started\n");

    #define ERR_ACT close (tcpserv_fd)

    // Send port to broadcast
    uint16_t tcpserv_port = ntohs (addrGetPort (&tcpserv_addr));
    printf ("TCP server port %d\n", tcpserv_port);

    CHECK_ERR (SendDataBroadcast (&tcpserv_addr, sizeof (tcpserv_addr)), ERR_ACT);

    // Accept of clients
    sigset_t sigset_noalarm = {0};
    sigfillset (&sigset_noalarm);
    sigdelset  (&sigset_noalarm, SIGALRM);

    struct sigaction alaram_act = {0};
    bzero (&alaram_act, sizeof (alaram_act));
    alaram_act.sa_handler = callback_alarm;
    sigfillset (&alaram_act.sa_mask);

    CHECK_ERR (sigaction (SIGALRM, &alaram_act, NULL), ERR_ACT);
    CHECK_ERR (sigprocmask (SIG_BLOCK, &sigset_noalarm, NULL), ERR_ACT);

    alarm (5);

    int clifds[clients_limit] = {0};
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