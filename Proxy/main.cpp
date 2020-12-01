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
        printf ("Error input. Please enter: numberChild nameFile\n");
        return 0;
    }

    errno = 0;
    const int n = strtol (argv[1], nullptr, 10);
    if (errno || n <= 0) {
        printf ("Error input. Please enter: numberChild > 0 nameFile\n");
        return 0;
    }

    pid_t pid = 0;
    int sizeBuf = 0, ret = 0;
    int pipe_sc[2] = {}, pipe_cs[2] = {};   // Server->Client, Client->Server

    CHECK_ERROR (pipe (pipe_cs));

    sizeBuf = SizeBuf (0, n);
    pid = fork ();
    CHECK_ERROR (pid);

    if (pid == 0) {     // Child
        close (pipe_cs[0]);
        int temp = 0;   // For FIFO
        ret = write (pipe_cs[1], &temp, sizeof (temp));
        CHECK_ERROR (ret);

        int fdFile = open (argv[2], O_RDONLY);
        CHECK_ERROR (fdFile);

        char buf[sizeBuf];
        if (sizeBuf > PIPE_BUF)
            sizeBuf = PIPE_BUF;

        while ((ret = read (fdFile, buf, sizeBuf))) {
            CHECK_ERROR (ret);
            write (pipe_cs[1], buf, ret);
        }

        close (fdFile);
        close (pipe_cs[1]);

        printf ("First finished\n");
        return 0;
    }

    channel_t channels[n];
    memset (channels, 0, sizeof (channels));

    // Connect to the first child -----------------------------

    close (pipe_cs[1]);

    pollfd checkConnect = {};
    checkConnect.fd = pipe_cs[0];
    checkConnect.events = POLLIN;
    ret = poll (&checkConnect, 1, timeConnect);
    CHECK_ERROR (ret);

    if (ret == 0) { // Time is out
        perror ("poll");
        printf ("Failed to connect to the first child!\n");

        close (pipe_cs[0]);
        return 0;
    }

    int temp = 0;   // For FIFO
    ret = read (pipe_cs[0], &temp, sizeof (temp));
    printf ("Good!\n");
    CHECK_ERROR (ret);

    channels[0].m_fdRead  = pipe_cs[0];
    channels[0].m_sizeBuf = sizeBuf;
    channels[0].m_curPos = -1;

    // Connect to other childes -------------------------------
    int numChannel = 0;
    for (int i = 1; i < n; i++) {
        CHECK_ERROR (pipe (pipe_cs) || pipe (pipe_sc));

        sizeBuf = SizeBuf (i, n);

        pid = fork ();
        CHECK_ERROR (pid);

        if (pid == 0) {     // Child
            close (pipe_sc[1]);
            close (pipe_cs[0]);

            numChannel = i;

            break;
        } else {            // Parent
            close (pipe_sc[0]);
            close (pipe_cs[1]);

            channels[i].m_fdRead  = pipe_cs[0];
            channels[i].m_fdWrite = pipe_sc[1];
            channels[i].m_sizeBuf = sizeBuf;
            channels[i].m_curPos  = -1;
        }
    }

    // Base code of work of child -----------------------------

    if (pid == 0) { // Child
        printf ("start %d\n", pipe_sc[1]);
        char buf[sizeBuf];
        while ((ret = read (pipe_sc[0], buf, sizeBuf))) {
            printf ("channel[%d]: read \"%s\"\n", numChannel, buf);
            CHECK_ERROR (ret);
            CHECK_ERROR (write (pipe_cs[1], buf, ret));
        }

        printf ("end\n");
        return 0;
    }

    // Init channel buffers -----------------------------------

    int totalSizeBuf = 0;
    for (int i = 0; i < n; i++)
        totalSizeBuf += channels[i].m_sizeBuf;

    char channelBuf[totalSizeBuf];
    memset (channelBuf, 0, sizeof (channelBuf));

    char *curPosBuf = channelBuf;
    for (int i = 0; i < n; i++) {
        channels[i].m_buf = curPosBuf;
        curPosBuf += channels[i].m_sizeBuf;
    }

    // Base code of work of parent ----------------------------

    pollfd polls[2 * (n - 1)];
    memset (polls, 0, sizeof (polls));

    for (int i = 0; i < n; i++) {
        polls[2 * i].fd = channels[i - 1].m_fdRead;
        polls[2 * i].events = POLLIN;

        polls[2 * i + 1].fd = channels[i - 1].m_fdWrite;
        polls[2 * i + 1].events = POLLOUT;
    }

    int sizeDataFile = 0;
    while ((sizeDataFile = read (channels[0].m_fdRead, channels[0].m_buf, channels[0].m_sizeBuf))) {
        CHECK_ERROR (sizeDataFile);

        channels[0].m_curPos = sizeDataFile - 1;

        while (channels[0].m_curPos > -2) {
            ret = poll (polls, 2 * n, timeConnect);
            CHECK_ERROR (ret);

            if (ret == 0) {
                printf ("Time is out: %d\n", __LINE__);
                CloseChannels (channels, n);
                return 0;
            }

            for (int i = 1; i < n; i++) {
                switch (polls[2 * i].events) {
                    case POLLIN:
                    {
                        if (channels[i].m_curPos == -1) {
                            ret = read (channels[i].m_fdRead, channels[i].m_buf, channels[i].m_sizeBuf);
                            channels[i].m_curPos = ret - 1;
                            CHECK_ERROR (ret);
                        }
                    } break;
                }

                switch (polls[2 * i + 1].events) {
                    case POLLOUT:
                    {
                        int sizeData = channels[i - 1].m_curPos + 1;
                        if (sizeData != 0) {
                            if (sizeData > channels[i].m_sizeBuf)
                                sizeData = channels[i].m_sizeBuf;

                            channels[i - 1].m_curPos -= sizeData;
                            ret = write (channels[i].m_fdWrite, channels[i - 1].m_buf, sizeData);
                            CHECK_ERROR (ret);
                        }
                    } break;
                }

                polls[2 * i].revents = 0;
                polls[2 * i + 1].revents = 0;
            }

            if (channels[n - 1].m_curPos != -1) {
                write (STDOUT_FILENO, channels[n - 1].m_buf, channels[n - 1].m_curPos + 1);
                channels[n - 1].m_curPos = 0;
            }
        }
    }
}