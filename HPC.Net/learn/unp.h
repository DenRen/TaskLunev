#ifndef UNP_H
#define UNP_H

#include "../../HPC_Integral/lib/debug_func.h"
#include "net_wrapper.h"

#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>

#include <sys/types.h>
#include <sys/time.h>
#include <arpa/inet.h>

#include <stdbool.h>

#include <strings.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

const int MAX_LINE = 256;
const int BUF_SIZE = 512;

#define SA struct sockaddr

#define CHECK_ERR(act, err_act)     \
    if ((act) < 0) {                \
        PRINT_ERROR (#act);         \
        do { err_act; } while (0);  \
        return -1;                  \
    }

#endif // UNP_H