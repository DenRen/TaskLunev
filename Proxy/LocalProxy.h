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
#include "CircBuffer.h"

struct ProxyClient {
    int m_fdWrite;
    int m_fdRead;
};

struct ProxyChannel {
    int m_fdWrite;
    int m_fdRead;

    char *m_buf;
    char *m_curPos;
    int m_size;
    int m_capacity;

    bool m_full;
};

struct ProxyServer {
    ProxyChannel *m_channels;
    int m_numberChannels;

    int m_fdAddClientServer;

    int m_totalSizeBuf;
    char *m_totalBuffer;
};

int StartProxy (ProxyServer *proxyServer, int numberClient);
void CloseProxy (ProxyServer *proxyServer);
int StartFirstClient (ProxyClient proxyClient, int fdAddClientServer);
int StartClient (ProxyClient proxyClient);
int SendFile (const ProxyServer proxyServer, const char *pathFile);
int VerifierProxy (const ProxyServer * const proxyServer);

void DumpProxy (const ProxyServer *proxyServer);

int SetFlag (int fd, unsigned flag);
int RemoveFlag (int fd, unsigned flag);

int SetBlock (int fd);
int SetNonBlock (int fd);
#endif //PROXY_LOCALPROXY_H