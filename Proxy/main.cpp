#define TURN_DEBUG

#include "LocalProxy.h"

struct channel_t {
    int m_fdRead;
    int m_fdWrite;
    int m_sizeBuf;

    int m_curPos;
    char *m_buf;
};

inline int SizeBuf (int i, int n) {
    assert (i >= 0);
    assert (i < n);

    return pow (3, n - i) * 1000;
}

void CloseChannels (channel_t *channels, int size) {
    assert (size >= 0);

    close (channels[0].m_fdRead);
    while (size--) {
        close (channels[size].m_fdWrite);
        close (channels[size].m_fdRead);
    }
}

const int timeConnect = 1000;

int main (int argc, char *argv[]) {

    if (argc != 3) {
        printf ("Error input. Please enter: numberChild pathFile\n");
        return 0;
    }

    const int numberChild = GetNumber (argv[1]);
    if (errno || numberChild <= 0) {
        printf ("Error input. Please enter: numberChild > 0 pathFile\n");
        errno = 0;
        return 0;
    }

    ProxyServer proxyServer = {};
    const char *pathFile = argv[2];

    if (StartProxy (&proxyServer, numberChild) == -1) {
        CloseProxy (&proxyServer);

        printf ("Failed to create proxy\n");
        return 0;
    }

    if (SendFile (proxyServer, pathFile) == -1) {
        CloseProxy (&proxyServer);

        printf ("Failed to send file\n");
        return 0;
    }

    CloseProxy (&proxyServer);
}