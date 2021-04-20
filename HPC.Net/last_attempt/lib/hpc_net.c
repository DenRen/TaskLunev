#include "hpc_net.h"

#include "../../HPC_Integral/lib/hpc.h"
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
#include <stdbool.h>
#include <math.h>
#include <sys/sysinfo.h>
#include <fcntl.h>

#include <netinet/tcp.h>

// ===============\\
// Other functions -------------------------------------------------------------
// ===============//

double funcX (double x) {
    return x;
}

double funcX_mult_X (double x) {
    return x * x;
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

    close (sockfd);

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

static int sigalarm_flag = 0;
// Only for call EINT from accept
static void callback_alarm (int signum) {
    sigalarm_flag = 1;
    return;
}

static void callback_sigio (int signum) {
    printf ("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAa\n");
    perror ("");

    fflush (stdout);
    fflush (stderr);
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

static int CheckServAttr (serv_attr_t* serv_attr, bool print_err) {
    if (serv_attr->clients_limit <= 0 || serv_attr->if_name == NULL) {

        IF_DEBUG (print_err = true);
        if (print_err) {
            PRINT_ERROR ("clients_limit <= 0 || if_name == NULL");
        }

        return -1;
    }

    if (serv_attr->sa_family != AF_INET) { // Add && family != AF_INET6

        IF_DEBUG (print_err = true);
        if (print_err) {
            char errstr[MAXLEN_ERRSTR] = {0}; 
            snprintf (errstr, sizeof (errstr),
                    "Socket family %d is not supported!\n", serv_attr->sa_family);
            PRINT_ERROR (errstr);
        }

        return -1;
    }

    return 0;
}

// In sockfd set SO_SNDTIMEO
static int SendKeepAliveProbe (int sockfd) {
    int buf = 0;
    printf ("S_\n");
    int state = write (sockfd, &buf, 0);
    printf ("F_\n");
    perror ("AAA");
    if (state == 0 || state == -1)
        return -1;
    
    return 0;
}

int writen (int fd, void* data, size_t len) {
    if (fd < 0 || data == NULL) {
        PRINT_ERROR ("fd < 0 || buf == NULL");
        return -1;
    }

    int num_written = 0;
    do {
        num_written = write (fd, data, len);
        if (num_written == -1 || num_written == 0)
            CHECK_ERR (-1, )
        
        *((char**) &data) += num_written;

    } while ((len -= num_written));

    return 0;
}

int readn (int fd, void* data, size_t len) {
    if (fd < 0 || data == NULL) {
        PRINT_ERROR ("fd < 0 || buf == NULL");
        return -1;
    }

    int num_readden = 0;
    do {
        num_readden = read (fd, data, len);
        if (num_readden == -1 || num_readden == 0)
            CHECK_ERR (-1, )

        *((char**) &data) += num_readden;

    } while ((len -= num_readden));

    return 0;
}

// comp_units != NULL
static int _dumpCompUnits (comp_unit_t* comp_units, size_t num_units) {
    printf ("Dump compute units:\n"
            "fd -> num logic cpu\n");
    for (size_t num = 0; num < num_units; ++num)
        printf ("%2d -> %d\n", comp_units[num].sock_fd, comp_units[num].num_logic_cpu);
    
    return 0;
}

// ===================================\\
// Start/Close Client/Server functions -------------------------------------------------------------
// ===================================//

static int SetSocketKeepAlive (int sockfd) {
    const int use = 1;
    const int cnt = 2;

    CHECK_ERR (setsockopt (sockfd, SOL_SOCKET,  SO_KEEPALIVE,  (const void*) &use, sizeof (use)), );
    CHECK_ERR (setsockopt (sockfd, IPPROTO_TCP, TCP_KEEPIDLE,  (const void*) &use, sizeof (use)), );
    CHECK_ERR (setsockopt (sockfd, IPPROTO_TCP, TCP_KEEPINTVL, (const void*) &use, sizeof (use)), );
    CHECK_ERR (setsockopt (sockfd, IPPROTO_TCP, TCP_KEEPCNT,   (const void*) &cnt, sizeof (cnt)), );

    CHECK_ERR (setsockopt (sockfd, IPPROTO_IP, IP_RECVERR,   (const void*) &cnt, sizeof (cnt)), );

    //fcntl(sockfd, F_SETFL, O_ASYNC | fcntl(sockfd, F_GETFL));
    //fcntl(sockfd, F_SETOWN, getpid());

    return 0;
}

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

    CHECK_ERR (SetSocketKeepAlive (tcpcli_fd), close (tcpcli_fd));

    return tcpcli_fd;
}

static int SetSignalHandlers () {
    struct sigaction alaram_act = {0};
    bzero (&alaram_act, sizeof (alaram_act));
    alaram_act.sa_handler = callback_alarm;
    sigfillset (&alaram_act.sa_mask);

    struct sigaction sigio_act = {0};
    bzero (&sigio_act, sizeof (sigio_act));
    sigio_act.sa_handler = callback_sigio;
    sigfillset (&sigio_act.sa_mask);
    
    //CHECK_ERR (sigaction (SIGIO,   &sigio_act,  NULL), );
    CHECK_ERR (sigaction (SIGALRM, &alaram_act, NULL), );

    return 0;
}

int StartServer (serv_attr_t* serv_attr, serv_param_t* serv_param) {
    CHECK_PTR (serv_param);

    // Get server attribute
    CHECK_ERR (CheckServAttr (serv_attr, true), );
    const int         clients_limit = serv_attr->clients_limit;
    const char*       if_name       = serv_attr->if_name;
    const sa_family_t sa_family     = serv_attr->sa_family;

    // Get static IP from if_name and sa_family
    char host[NI_MAXHOST] = {0};
    CHECK_ERR (GetStaticAddr (if_name, sa_family, host, sizeof (host)), );

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
    sigset_t sigset_noalarm = {0}, sigset_old = {0};
    sigfillset (&sigset_noalarm);
    sigdelset  (&sigset_noalarm, SIGALRM);  // Unblock only SIGALRM

    //CHECK_ERR (sigprocmask (SIG_BLOCK, &sigset_noalarm, &sigset_old), ERR_ACT);
    CHECK_ERR (SetSignalHandlers (), ERR_ACT);
    
    // Accept of clients
    int* clifds = (int*) calloc (clients_limit, sizeof (int));
    CHECK_NULL (clifds, ERR_ACT);
    
    alarm (5);  // Max time wait loop of accept

    #undef ERR_ACT
    #define ERR_ACT close (tcpserv_fd); free (clifds)

    int numcli = 0;
    for (; numcli < clients_limit; ++numcli) {
        if ((clifds[numcli] = accept (tcpserv_fd, NULL, NULL)) == -1) {
            if (errno == EINTR) {
                if (sigalarm_flag)
                    break;
                else {
                    numcli--;
                    continue;
                }
            }
            else
                CHECK_ERR (-1, ERR_ACT);
        }

        CHECK_ERR (SetSocketKeepAlive (clifds[numcli]), ERR_ACT);
    }

    alarm (0);
    errno = 0;

    //CHECK_ERR (sigprocmask (SIG_SETMASK, &sigset_old, NULL), ERR_ACT);

    IF_DEBUG (printf ("Number clients: %d\n", numcli));

    // Preparation and save of values
    int* tmp_clifds = (int*) realloc (clifds, numcli * sizeof (int));
    if (tmp_clifds == NULL)
        CHECK_ERR (-1, printf ("Realloc failed! May be number clients is zero\n"); 
                       close (tcpserv_fd); free (tmp_clifds));
    clifds = tmp_clifds;

    if ((serv_param->comp_units = (comp_unit_t*) calloc (numcli, sizeof (comp_unit_t))) == NULL)
        CHECK_ERR (-1, ERR_ACT);

    CHECK_ERR (InitCompUnits (serv_param->comp_units, numcli, clifds), ERR_ACT);

    IF_DEBUG (_dumpCompUnits (serv_param->comp_units, numcli));

    serv_param->num_clients = numcli;
    serv_param->serv_fd = tcpserv_fd;
    
    free (tmp_clifds);

    #undef ERR_ACT

    return 0;
}

// ==============\\
// Math functions -------------------------------------------------------------
// ==============//

int InitCompUnits (comp_unit_t* comp_units, int num_units, int* fds) {
    for (int numcli = 0; numcli < num_units; ++numcli) {
        comp_units[numcli].sock_fd = fds[numcli];
        int* num_logic_cpu = &comp_units[numcli].num_logic_cpu;
        // Maybe we need SO_RCVTIMEO
        CHECK_ERR (readn (fds[numcli], num_logic_cpu, sizeof (*num_logic_cpu)), );
    }

    return 0;
}

// ptask != NULL
static hpc_int_arg_t CompUnitTask2IntArg (comp_unit_task_t* ptask) {
    hpc_int_arg_t int_arg = {
        .a = ptask->a,
        .b = ptask->b,
        .func = NULL,
        .num_threads = ptask->num_threads
    };

    switch (ptask->type_func) {
        case X:        int_arg.func = funcX;        break;
        case X_mult_X: int_arg.func = funcX_mult_X; break;
    }

    return int_arg;
}

int hpcnetStartComputeUnit (int client_fd) {
    // Send number of logic cpu to server
    int num_logic_cpu = get_nprocs ();
    CHECK_ERR (writen (client_fd, &num_logic_cpu, sizeof (num_logic_cpu)), ); 

    IF_DEBUG (printf ("Client send number of logic cpu to server\n"));

    // Get and do task
    comp_unit_task_t task = {0};
    CHECK_ERR (readn (client_fd, &task, sizeof (task)), 0);
    IF_DEBUG (printf ("Client recieve task from server\n"));
    
    hpc_int_arg_t int_arg = CompUnitTask2IntArg (&task);
    double result = hpcIntegral (&int_arg);

    // Send answer
    CHECK_ERR (writen (client_fd, &result, sizeof (result)), );

    return 0;
}

int hpcnetCalcIntegral (serv_param_t* serv_param, unsigned num_threads,
                        double (*func) (double), double a, double b, double* result) {
    if (ServParamIsFull (serv_param) == false) {
        PRINT_ERROR ("ServParamIsFull (serv_param) is false\n");
        errno = EINVAL;
        return NAN;
    }

    int num_clients = serv_param->num_clients;
    comp_unit_t *comp_units = serv_param->comp_units;

    double len = (b - a) / num_clients;

    comp_unit_task_t task = {
        .a = a,
        .b = a,
        .type_func = X,
        .num_threads = num_threads
    };

    for (int i = 0; i < num_clients; ++i) {
        task.a = task.b;
        task.b = task.a + len;
        CHECK_ERR (writen (comp_units[i].sock_fd, &task,  sizeof (task)), );
        IF_DEBUG (printf ("A task has been sent to the client[%d]\n"););
    }

    *result = 0;

    for (int i = 0; i < num_clients; ++i) {
        double cli_res = 0.0f;
        CHECK_ERR (readn (comp_units[i].sock_fd, (void*) &cli_res, sizeof (cli_res)), );
        *result += cli_res;
    }
    
    return 0;
}

int CloseServer (serv_param_t* serv_param) {
    CHECK_ERR (ServParamIsFull (serv_param) - 1, );
    
    const int num_clients = serv_param->num_clients;
    const comp_unit_t* comp_units = serv_param->comp_units;

    for (int num_cli = 0; num_cli < num_clients; ++num_cli)
        close (comp_units[num_cli].sock_fd);

    close (serv_param->serv_fd);

    return 0;
}

bool ServAttrIsFull (serv_attr_t* serv_attr) {
    return serv_attr                != NULL &&
           serv_attr->if_name       != NULL &&
           serv_attr->clients_limit >=0;
}

bool ServParamIsFull (serv_param_t* serv_param) {
    return serv_param              != NULL  &&
           serv_param->serv_fd     >= 0     &&
           serv_param->num_clients > 0      &&
           serv_param->comp_units  != NULL;
}