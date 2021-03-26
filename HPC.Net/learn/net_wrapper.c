#include "net_wrapper.h"

#define DEBUG
#include "../../HPC_Integral/lib/debug_func.h"

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
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

char* sock_ntop (const struct sockaddr* sa, socklen_t salen) {
    if (sa == NULL) {
        PRINT_ERROR ("sa == NULL");
        errno = EINVAL;
        return NULL;
    }

    static char port_str[7 + 1 + 1];
    static char addr_str[128];
    switch (sa->sa_family) {
        case AF_INET: {
            const struct sockaddr_in* sa_in = (const struct sockaddr_in*) sa;
            
            if (inet_ntop (AF_INET, &sa_in->sin_addr, addr_str, sizeof (addr_str)) == NULL) {
                PRINT_ERROR ("inet_ntop");
                return NULL;
            }
            const uint16_t port = ntohs (sa_in->sin_port);
            if (port != 0) {
                snprintf (port_str, sizeof (port_str), ":%d", port);
                strcat (addr_str, port_str);
            }

            return addr_str;

        } break;
        case AF_INET6: {
            const struct sockaddr_in6* sa_in6 = (const struct sockaddr_in6*) sa;
            
            if (inet_ntop (AF_INET, &sa_in6->sin6_addr, addr_str, sizeof (addr_str)) == NULL) {
                PRINT_ERROR ("inet_ntop");
                return NULL;
            }
            const uint16_t port = ntohs (sa_in6->sin6_port);
            if (port != 0) {
                snprintf (port_str, sizeof (port_str), ":%d", port);
                strcat (addr_str, port_str);
            }

            return addr_str;

        } break;
    default:
        return NULL;
    }
}