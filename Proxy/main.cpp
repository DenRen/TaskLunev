#define TURN_DEBUG

#include "LocalProxy.h"

#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

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

const int timeConnect = 1000;

int main (int argc, char *argv[]) {

/*
    printf ("%d\n", getpid ());
    sleep (60);

    char path[128] = "";
    sprintf (path, "/proc/%d/fd/", getpid ());
    fs::directory_iterator it (path);
    int count = 0;
    for (auto &p : it) {
        printf ("%s ", p.path ().filename ().c_str ());
        count++;
    }
    printf ("-> %d\n", count);
    printf ("++++++++++++++++\n");
    printf ("%d\n", getpid ());
    sleep (60);
*/
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
    proxyServer.m_channels = nullptr;
    proxyServer.m_buffers  = nullptr;

    const char *pathFile = argv[2];

    int ret = StartProxy (&proxyServer, numberChild);
    if (ret == -1) {
        CloseProxy (&proxyServer);

        printf ("Failed to create proxy\n");
        return 0;
    } else if (ret == 1) {  // Return proxy client
        printf ("Return child\n");
        fflush (stdout);
        return 0;
    }

    if (SendFile (proxyServer, pathFile) == -1) {
        CloseProxy (&proxyServer);

        printf ("Failed to send file\n");
        return 0;
    }

    CloseProxy (&proxyServer);
}