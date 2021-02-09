#define TURN_DEBUG

#include "LocalProxy.h"

struct channel_t {
    int m_fdRead;
    int m_fdWrite;
    int m_sizeBuf;

    int m_curPos;
    char *m_buf;
};

void CloseChannels (channel_t *channels, int size) {
    assert (size >= 0);

    close (channels[0].m_fdRead);
    while (size--) {
        close (channels[size].m_fdWrite);
        close (channels[size].m_fdRead);
    }
}

int main (int argc, char *argv[]) {
    if (argc != 3) {
        printf ("Error input. Please enter: numberChild pathFile\n");
        return 0;
    }

    const int numberChild = GetNumber (argv[1]);
    if (errno || numberChild <= 0) {
        printf ("Error input. Please enter: numberChild pathFile\n");
        errno = 0;
        return 0;
    }

    ProxyServer proxyServer = {};
    proxyServer.m_channels = nullptr;

    int ret = StartProxy (&proxyServer, numberChild);
    if (ret == -1) {
        CloseProxy (&proxyServer);

        printf ("Failed to create proxy\n");
        return 0;
    }

    // You can send any number of files, f.e. one
    const char *pathFile = argv[2];

    if (SendFile (proxyServer, pathFile) == -1) {
        CloseProxy (&proxyServer);

        printf ("Failed to send file\n");
        return 0;
    }

    CloseProxy (&proxyServer);
}