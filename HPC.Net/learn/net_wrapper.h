#ifndef NET_WRAPPER_H
#define NET_WRAPPER_H

#include <sys/socket.h>

char* sock_ntop (const struct sockaddr* sa, socklen_t salen);

#endif // NET_WRAPPER_H