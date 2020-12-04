#include "LocalProxy.h"
#include "CircBuffer.h"
#include <csignal>

inline int CalcSizeBuf (int i, int n) {
    MYASSERT (i >= 0);
    MYASSERT (i < n);

    return pow (3, n - i) * 1000;
}

int StartProxy (ProxyServer *proxyServer, int numberClient) {

    if (VerifierProxy (proxyServer) == true) {
        errno = EEXIST;
        DUMP_DEBUG_INFO;
        return -1;
    }

    // Allocate proxy server p.1 ------------------------------------------------------------------

    proxyServer->m_channels = (ProxyChannel *) calloc (numberClient, sizeof (ProxyChannel));
    if (proxyServer->m_channels == nullptr) {
        errno = ENOMEM;
        DUMP_DEBUG_INFO;
        return -1;
    }
    proxyServer->m_numberChannels = numberClient;
    ProxyChannel *channels = proxyServer->m_channels;

    //  -----------------------------------------------------------------------------

    int ret = 0;
    int fdClientServer[2] = {}, fdServerClient[2] = {}, fdAddClientServer[2] = {};
    for (int i = 0; i < numberClient; i++) {
        CHECK_ERROR (pipe2 (fdServerClient, O_NONBLOCK) || pipe2 (fdClientServer, O_NONBLOCK));
        if (i == 0) {
            CHECK_ERROR (pipe (fdAddClientServer));
        }

        ret = fork ();
        CHECK_ERROR (ret);

        if (ret == 0) {                     // Child
            for (int j = 0; j < i; j++) {
                close (channels[j].m_fdWrite);
                close (channels[j].m_fdRead);
            }

            close (fdServerClient[1]);
            close (fdClientServer[0]);

            ProxyClient proxyClient = {};
            proxyClient.m_fdWrite = fdClientServer[1];
            proxyClient.m_fdRead  = fdServerClient[0];

            if (i == 0) {
                close (fdAddClientServer[0]);
                StartFirstClient (proxyClient, fdAddClientServer[1]);
            } else {
                StartClient (proxyClient);
            }

            exit (EXIT_SUCCESS);
        }

        if (i == 0) {
            close (fdAddClientServer[1]);
            proxyServer->m_fdAddClientServer = fdAddClientServer[0];
        }

        close (fdServerClient[0]);
        close (fdClientServer[1]);

        //SetNonBlock (fdServerClient[1]);
        SetBlock (channels[i].m_fdRead);
        channels[i].m_fdWrite = fdServerClient[1];
        channels[i].m_fdRead  = fdClientServer[0];
    }

    //Allocate proxy server p.2 ------------------------------------------------------------------

    int totalSizeBuf = 0;
    for (int i = 0; i < numberClient; i++) {
        int curSizeBuf = CalcSizeBuf (i, numberClient);

        channels[i].m_capacity = curSizeBuf;
        channels[i].m_size = 0;
        totalSizeBuf += curSizeBuf;
    }
    proxyServer->m_totalSizeBuf = totalSizeBuf;

    proxyServer->m_totalBuffer = (char *) calloc (totalSizeBuf, sizeof (char));
    if (proxyServer->m_totalBuffer == nullptr) {
        errno = ENOMEM;
        DUMP_DEBUG_INFO;
        return -1;
    }

    char *buffers = proxyServer->m_totalBuffer;
    for (int i = 0; i < numberClient; i++) {
        channels[i].m_buf = buffers;
        channels[i].m_curPos = buffers;
        buffers += channels[i].m_capacity;
    }

    return 0;
}

void CloseProxy (ProxyServer *proxyServer) {
    if (proxyServer->m_totalBuffer  != nullptr &&
        proxyServer->m_channels     != nullptr) {

        for (int i = 0; i < proxyServer->m_numberChannels; i++) {
            close (proxyServer->m_channels[i].m_fdRead);
            close (proxyServer->m_channels[i].m_fdWrite);
        }
    }

    close (proxyServer->m_fdAddClientServer);

    free (proxyServer->m_totalBuffer);
    free (proxyServer->m_channels);

    proxyServer->m_totalBuffer = nullptr;
    proxyServer->m_channels = nullptr;
    proxyServer->m_totalSizeBuf = 0;
}

int SendFile (const ProxyServer proxyServer, const char *pathFile) {
    CHECK_TRUE (VerifierProxy (&proxyServer));
    MYASSERT (pathFile != nullptr);

    int ret = 0;

    // Send path file first client ----------------------------------------------------------------
    ret = write (proxyServer.m_channels[0].m_fdWrite, pathFile, strlen (pathFile));
    CHECK_ERROR (ret);
    CHECK_TRUE (ret != strlen (pathFile) + 1);

    // Start system send file ---------------------------------------------------------------------
    const int numberChannels = proxyServer.m_numberChannels;
    ProxyChannel *channels = proxyServer.m_channels;

    pollfd polls[2 * numberChannels];
    memset (polls, 0, sizeof (polls));

    for (int i = 0; i < numberChannels; i++) {
        polls[2 * i].fd = channels[i].m_fdWrite;
        polls[2 * i].events = POLLHUP;

        polls[2 * i + 1].fd = channels[i].m_fdRead;
        polls[2 * i + 1].events = POLLHUP;
    }

    polls[0].fd = proxyServer.m_fdAddClientServer;

    long counter = 0;
    long sizeFile = LONG_MAX;

    while (counter < sizeFile) {
        for (int i = 0; i < numberChannels; i++) {

            if (channels[i - 1].m_size != 0)
                polls[2 * i].events |= POLLOUT;
            else
                polls[2 * i].events &= ~POLLOUT;

            if (channels[i].m_size == 0)
                polls[2 * i + 1].events |= POLLIN;
            else
                polls[2 * i + 1].events &= ~POLLIN;
        }
        polls[0].events |= POLLIN;

        int numPolls = poll (polls, 2 * numberChannels, -1);
        CHECK_ERROR (numPolls);

        if (numPolls == 0) {
            printf ("\n------------------ Time is out --------------------\n");
            CHECK_TRUE (false);
        }

        if (polls[0].revents & POLLIN) {
            CHECK_ERROR (read (polls[0].fd, &sizeFile, sizeof (sizeFile)));
        }

        for (int i = 0; i < numberChannels && numPolls; i++)
        {
            if (polls[2 * i].revents & POLLHUP || polls[2 * i + 1].revents & POLLHUP) {
                printf ("%d: channel is out\n", i);
                return 0;
            }

            if (polls[2 * i + 1].revents & POLLIN) {

                ret = read (channels[i].m_fdRead, channels[i].m_buf, channels[i].m_capacity);
                if (ret == -1) {
                    CHECK_ERROR (ret);
                }
                channels[i].m_size = ret;

                if (i == numberChannels - 1) {
                    ret  = write (STDOUT_FILENO, channels[i].m_buf, ret);
                    CHECK_ERROR (ret);

                    channels[i].m_size -= ret;
                    counter += ret;
                }

                numPolls--;
            }

            if (polls[2 * i].revents & POLLOUT) {

                int size  = channels[i - 1].m_size;
                char *buf = channels[i - 1].m_curPos;
                int len = 16;
                while (size > 0) {
                    if (size < len)
                        len = size;

                    ret = write (channels[i].m_fdWrite, buf, len);
                    if (ret == -1) {
                        if (errno == EAGAIN) {
                            channels[i].m_full = true;
                            break;
                        } else {
                            CHECK_ERROR (ret);
                        }
                    }

                    buf += ret;
                    size -= ret;
                }

                channels[i - 1].m_size = size;
                if (size == 0) {
                    channels[i - 1].m_curPos = channels[i - 1].m_buf;
                } else {
                    channels[i - 1].m_curPos = buf;
                }
                numPolls--;
            }

            polls[2 * i].revents = 0;
            polls[2 * i + 1].revents = 0;
        }
    }

    return 0;
}

int VerifierProxy (const ProxyServer *const proxyServer) {
    return proxyServer->m_channels != nullptr;
}

int StartFirstClient (ProxyClient proxyClient, int fdAddClientServer) {
    int ret = 0;

    char nameFile[256] = "";
    SetBlock (proxyClient.m_fdWrite);
    SetBlock (proxyClient.m_fdRead);

    while (true) {
        ret = read (proxyClient.m_fdRead, nameFile, 256);
        CHECK_ERROR (ret);
        if (ret == 0) {
            return 0;
        }

        int fdFile = open (nameFile, O_RDONLY);
        CHECK_ERROR (fdFile);
        long sizeFile = 0;
        while (true) {
            ret = splice (fdFile,                nullptr,
                          proxyClient.m_fdWrite, nullptr,
                          1280, SPLICE_F_MOVE);
            CHECK_ERROR (ret);

            sizeFile += ret;

            if (ret == 0) {
                break;
            }
        }

        ret = write (fdAddClientServer, &sizeFile, sizeof (sizeFile));
        CHECK_ERROR (ret);
    }

}

int StartClient (ProxyClient proxyClient) {
    int ret = 0;

    SetBlock (proxyClient.m_fdRead);
    while (true) {

        ret = splice (proxyClient.m_fdRead,  nullptr,
                      proxyClient.m_fdWrite, nullptr,
                      1024 * 16, SPLICE_F_MOVE);
        CHECK_ERROR (ret);

        if (ret == 0) {
            printf ("%d: finished\n", getpid ());
            fflush (stdout);
            break;
        }
    }

    return 0;
}

int SetFlag (int fd, unsigned flag) {
    int flags = fcntl (fd, F_GETFL, 0);
    CHECK_ERROR (flags);

    if ((flags & flag) == false) {
        flags |= flag;

        flags = fcntl (fd, F_SETFL, 0);
        CHECK_ERROR (flags);
    }
    return EXIT_SUCCESS;
}

int RemoveFlag (int fd, unsigned flag) {
    int flags = fcntl (fd, F_GETFL, 0);
    CHECK_ERROR (flags);

    if (flags & flag) {
        flags &= ~flag;

        flags = fcntl (fd, F_SETFL, 0);
        CHECK_ERROR (flags);
    }
    return EXIT_SUCCESS;
}

int SetBlock (int fd) {
    return RemoveFlag (fd, O_NONBLOCK);
}

int SetNonBlock (int fd) {
    return SetFlag (fd, O_NONBLOCK);
}

void DumpProxy (const ProxyServer *proxyServer) {
    if (VerifierProxy (proxyServer)) {
        for (int i = 0; i < proxyServer->m_numberChannels; i++) {
            ProxyChannel *channel = &proxyServer->m_channels[i];
            printf ("%d %d\n", channel->m_fdWrite, channel->m_fdRead);
        }
    }
}
