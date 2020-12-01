#ifndef PROXY_LOCALPROXY_H
#define PROXY_LOCALPROXY_H

#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <unistd.h>
#include <cmath>
#include <cstring>
#include <poll.h>
#include <fcntl.h>
#include <cassert>
#include <climits>

#include "AddFunc.h"
#include "DebugFunc.h"

struct ProxyClient {
    int m_fdWrite;
    int m_fdRead;
};

struct ProxyChannel {
    int m_fdWrite;
    int m_fdRead;

    int m_sizeBuf;
    char *m_buf;
};

struct ProxyServer {
    ProxyChannel *m_channels;
    int m_numberChannels;

    int m_sizeBuf;
    char *m_buffers;
};

int StartProxy (ProxyServer *proxyServer, int numberClient);
void CloseProxy (ProxyServer *proxyServer);
int StartClient (ProxyClient proxyClient, bool isFirstChild = false);
int SendFile (const ProxyServer proxyServer, const char *pathFile);
int VerifierProxy (const ProxyServer * const proxyServer);

#endif //PROXY_LOCALPROXY_H