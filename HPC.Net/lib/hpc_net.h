#ifndef HPC_NET_H
#define HPC_NET_H

#define DEBUG
#include "../../HPC_Integral/lib/debug_func.h"
#include <stdint.h>
#include <sys/socket.h>
#include <stdbool.h>

#define SA struct sockaddr

#define CHECK_ERR(act, err_act) \                 
    do {                        \
        if ((act) < 0) {        \
            err_act;            \
            PRINT_ERROR (#act); \   
            return -1;          \
        }                       \
    } while (0);               
    
#define CHECK_NULL(act, err_act)    \                 
    do {                            \
        if (act == NULL) {          \
            err_act;                \
            PRINT_ERROR (#act);     \   
            return -1;              \
        }                           \
    } while (0); 

const char KEY_START_SERVER[] = "server";
const uint16_t SERV_PORT_MIN = 1025;
const uint16_t SERV_PORT_MAX = 64000;

const uint16_t UDP_BROADCAST_PORT = 8000;

const char LAN_IP_SERVER[] = "192.168.0.1";

const int CHECK_TIMEOUT = 30;

double funcX (double x);
double funcX_mult_X (double x);

enum TYPE_FUNC {
    X,
    X_mult_X
};

// ===============\\
// Main structures -------------------------------------------------------------
// ===============//

typedef struct {
    const int clients_limit;        // Max number of clients
    const char* if_name;            // Interphace name
    const sa_family_t sa_family;    // Socket address family
} serv_attr_t;

typedef struct {
    int sock_fd;
    int num_logic_cpu;
} comp_unit_t;          // Compute unit

typedef struct {
    int serv_fd;
    uint32_t num_clients;
    comp_unit_t* comp_units;
} serv_param_t;

typedef struct {
    double a, b, dx;
    int type_func;
    unsigned num_threads;
} comp_unit_task_t;

// =================\\
// Structure methods -------------------------------------------------------------
// =================//

bool ServAttrIsFull (serv_attr_t* serv_attr);
bool ServParamIsFull (serv_param_t* serv_param);

// ===================================\\
// Start/Close Client/Server functions -------------------------------------------------------------
// ===================================//

int StartClient ();
int StartServer (serv_attr_t* serv_attr, serv_param_t* serv_param);
int CloseServer (serv_param_t* serv_param);

// ==============\\
// Math functions -------------------------------------------------------------
// ==============//

int hpcnetStartComputeUnit (int client_fd);
int InitCompUnits (comp_unit_t* comp_units, int num_units, int* fds);

int hpcnetCalcIntegral (serv_param_t* serv_param, unsigned num_threads,
                        double (*func) (double), double a, double b, double eps, double* result);

// ===========================\\
// UDP Client/Server functions -------------------------------------------------------------
// ===========================//

int hpcnetBroadcastServerInit (struct sockaddr_in* pservaddr);
int hpcnetBroadcastClientInit (struct sockaddr_in* pservaddr);

// ===========================\\
// TCP Client/Server functions -------------------------------------------------------------
// ===========================//

int hpcnetTcpServerInit (char* host, int clients_limit, struct sockaddr_in* pservaddr);
int hpcnetTcpClientInit (const struct sockaddr_in* servaddr);

// ================\\
// System functions -------------------------------------------------------------
// ================//

int GetStaticAddr (const char* if_name, sa_family_t family, char str_host[], uint32_t len_host);

// ===============\\
// Other functions -------------------------------------------------------------
// ===============//

int getAddr (struct sockaddr_in* paddr);
int addrGetPort (const void* paddr);

#endif // HPC_NET_H