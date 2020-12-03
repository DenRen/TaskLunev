#include "LocalProxy.h"
#include "CircBuffer.h"
#include <csignal>
/*
{
pid_t pid = 0;
int sizeBuf = 0, ret = 0;
int pipe_sc[2] = {}, pipe_cs[2] = {};   // Server->Client, Client->Server

CHECK_ERROR (pipe (pipe_cs)
);

sizeBuf = SizeBuf (0, numberChild);
pid = fork ();
CHECK_ERROR (pid);

if (pid == 0) {     // Child
close (pipe_cs[0]);
int temp = 0;   // For FIFO
ret = write (pipe_cs[1], &temp, sizeof (temp));
CHECK_ERROR (ret);

int fdFile = open (argv[2], O_RDONLY);
CHECK_ERROR (fdFile);

char *buf = (char *) calloc (sizeBuf, sizeof (char));
CHECK_TRUE (buf
!= nullptr);

if (sizeBuf > PIPE_BUF)
sizeBuf = PIPE_BUF;

while ((
ret = read (fdFile, buf, sizeBuf)
)) {
CHECK_ERROR (ret);
write (pipe_cs[1], buf, ret
);
sleep (1);
//fprintf (stderr, "First writen: %s\numberChild", buf);
}

close (fdFile);
close (pipe_cs[1]);

//printf ("First finished\numberChild");
return 0;
}

channel_t channels[numberChild];
memset (channels,
0, sizeof (channels));

// Connect to the first child -----------------------------

close (pipe_cs[1]);

pollfd checkConnect = {};
checkConnect.
fd = pipe_cs[0];
checkConnect.
events = POLLIN;
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
//printf ("Good!\numberChild");
CHECK_ERROR (ret);

channels[0].
m_fdRead = pipe_cs[0];
channels[0].
m_sizeBuf = sizeBuf;
channels[0].
m_curPos = -1;

// Connect to other childes -------------------------------
int numChannel = 0;
for (
int i = 1;
i<numberChild;
i++) {
CHECK_ERROR (pipe (pipe_cs)
||
pipe (pipe_sc)
);

sizeBuf = SizeBuf (i, numberChild);

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

channels[i].
m_fdRead = pipe_cs[0];
channels[i].
m_fdWrite = pipe_sc[1];
channels[i].
m_sizeBuf = sizeBuf;
channels[i].
m_curPos = -1;
}
}

// Base code of work of child -----------------------------

if (pid == 0) { // Child
//printf ("start %d\numberChild", pipe_sc[1]);
char buf[sizeBuf];
while ((
ret = read (pipe_sc[0], buf, sizeBuf)
)) {
//printf ("channel[%d]: read %s\numberChild", numChannel, buf);
CHECK_ERROR (ret);
CHECK_ERROR (write (pipe_cs[1], buf, ret)
);
}

//printf ("end\numberChild");
return 0;
}

// Init channel buffers -----------------------------------

int totalSizeBuf = 0;
for (
int i = 0;
i<numberChild;
i++)
totalSizeBuf += channels[i].
m_sizeBuf;

char *channelBuf = (char *) calloc (totalSizeBuf, sizeof (char));
CHECK_TRUE (channelBuf
!= nullptr);

char *curPosBuf = channelBuf;
for (
int i = 0;
i<numberChild;
i++) {
channels[i].
m_buf = curPosBuf;
curPosBuf += channels[i].
m_sizeBuf;
}

// Base code of work of parent ----------------------------

pollfd *polls = (pollfd *) calloc (2 * numberChild, sizeof (pollfd));
CHECK_TRUE (polls
!= nullptr);

for (
int i = 0;
i<numberChild;
i++) {
polls[2 * i].
fd = channels[i].m_fdRead;
polls[2 * i].
events = POLLIN;

polls[2 * i + 1].
fd = channels[i].m_fdWrite;
polls[2 * i + 1].
events = POLLOUT;
}

int counterInputData = 0;
while (true) {
ret = poll (polls, 2 * numberChild, timeConnect);
CHECK_ERROR (ret);

if (ret == 0) {
printf ("Time is out");
CloseChannels (channels, numberChild
);
return 0;
}

for (
int i = 0;
i<numberChild;
i++) {
if (polls[2 * i].
revents &POLLIN
) {
if (channels[i].m_curPos == -1) {
ret = read (channels[i].m_fdRead, channels[i].m_buf, channels[i].m_sizeBuf);
channels[i].
m_curPos = ret - 1;
CHECK_ERROR (ret);

if (i == 0) {
counterInputData +=
ret;
}
}
}

if (i == 0)
continue;

if (polls[2 * i + 1].
revents &POLLOUT
&& channels[i].m_curPos == -1) {
int sizeData = channels[i - 1].m_curPos + 1;
if (sizeData != 0) {
if (sizeData > channels[i].m_sizeBuf)
sizeData = channels[i].m_sizeBuf;

channels[i - 1].m_curPos -=
sizeData;
ret = write (channels[i].m_fdWrite, channels[i - 1].m_buf, sizeData);
CHECK_ERROR (ret);
}
}
}

if (channels[numberChild - 1].m_curPos != -1) {
write (STDOUT_FILENO, channels[numberChild - 1]
.m_buf, channels[numberChild - 1].m_curPos + 1);
counterInputData -= channels[numberChild - 1].m_curPos + 1;
channels[numberChild - 1].
m_curPos = -1;
}

if (polls[0].revents ==
POLLHUP &&counterInputData
== 0) {
break;
}
}
*/

/*int sizeDataFile = 0;
while ((sizeDataFile = read (channels[0].m_fdRead, channels[0].m_buf, channels[0].m_sizeBuf))) {
    CHECK_ERROR (sizeDataFile);

    channels[0].m_curPos = sizeDataFile - 1;

    while (channels[0].m_curPos > -2) {
        ret = poll (polls, 2 * numberChild, timeConnect);
        CHECK_ERROR (ret);

        if (ret == 0) {
            printf ("Time is out: %d\numberChild", __LINE__);
            CloseChannels (channels, numberChild);
            return 0;
        }

        for (int i = 1; i < numberChild; i++) {
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

        if (channels[numberChild - 1].m_curPos != -1) {
            write (STDOUT_FILENO, channels[numberChild - 1].m_buf, channels[numberChild - 1].m_curPos + 1);
            channels[numberChild - 1].m_curPos = 0;
        }
    }
}

CloseChannels (channels, numberChild
);
//printf ("Parent finished\numberChild");
}*/

inline int CalcSizeBuf (int i, int n) {
    MYASSERT (i >= 0);
    MYASSERT (i < n);

    return pow (3, n - i) * 10;
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
    int fdClientServer[2] = {}, fdServerClient[2] = {};
    for (int i = 0; i < numberClient; i++) {
        CHECK_ERROR (pipe (fdServerClient) || pipe (fdClientServer));

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

            StartClient (proxyClient, i == 0);

            exit (EXIT_SUCCESS);
        }

        close (fdServerClient[0]);
        close (fdClientServer[1]);

        SetNonBlock (fdServerClient[1]);
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

    ret = write (proxyServer.m_channels[0].m_fdWrite, pathFile, strlen (pathFile));
    CHECK_ERROR (ret);
    CHECK_TRUE (ret != strlen (pathFile) + 1);

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

    polls[0].events = 0;
    polls[1].events = 0;

    int counter = 0;    // For count input and output byte
    int offset = 0;     // For ignore dead first

    while (true) {
        for (int i = 1; i < numberChannels; i++) {

            if (channels[i - 1].m_size != 0)
                polls[2 * i].events |= POLLOUT;
            else
                polls[2 * i].events &= ~POLLOUT;

            if (channels[i].m_size == 0)
                polls[2 * i + 1].events |= POLLIN;
            else
                polls[2 * i + 1].events &= ~POLLIN;
        }

        if (channels[0].m_size == 0)
            polls[1].events |= POLLIN;
        else
            polls[1].events &= ~POLLIN;

        if (counter == 0 && offset != 0) {
            return 0;
        }
        // todo: неправильные условия выхода
        int numPolls = poll (polls + 2 * offset, 2 * (numberChannels - offset), 1000);
        CHECK_ERROR (numPolls);

        if (numPolls == 0) {
            printf ("\nTime is out\n");
            CHECK_TRUE (false);
        }

        for (int i = 0; i < numberChannels && numPolls; i++)
        {
            if (polls[2 * i + 1].revents & POLLIN) {

                ret = read (channels[i].m_fdRead, channels[i].m_buf, channels[i].m_capacity);
                if (ret == -1) {
                    CHECK_ERROR (ret);
                }
                channels[i].m_size = ret;

                if (i == 0) {
                    counter += ret;
                }

                if (i == numberChannels - 1) {
                    ret  = write (STDOUT_FILENO, channels[i].m_buf, ret);
                    CHECK_ERROR (ret);

                    channels[i].m_size -= ret;
                    counter -= ret;
                }

                numPolls--;
            }

            if (polls[2 * i].revents & POLLOUT) {

                int size  = channels[i - 1].m_size;
                char *buf = channels[i - 1].m_buf;
                int len = 16;
                while (size > 0) {
                    if (size < len)
                        len = size;

                    ret = write (channels[i].m_fdWrite, buf, len);
                    if (ret == -1) {
                        if (errno == EAGAIN) {
                            channels[i].m_full = true;
                            break;
                        }
                        else
                            CHECK_ERROR (ret);
                    }

                    buf += ret;
                    size -= ret;
                }

                channels[i - 1].m_size = size;
                numPolls--;
            }

            if (polls[2 * i].revents & POLLHUP || polls[2 * i + 1].revents & POLLHUP) {
                if (i != 0) {
                    printf ("%d: channel is out\n", i);
                    return 0;
                } else {
                    offset = 0;
                    polls[0].revents = 0;
                    polls[1].revents = 0;
                    break;
                }
            }

            polls[2 * i].revents = 0;
            polls[2 * i + 1].revents = 0;
        }
    }
}

int VerifierProxy (const ProxyServer *const proxyServer) {
    return proxyServer->m_channels != nullptr;
}

int StartClient (ProxyClient proxyClient, bool isFirstChild) {
    int ret = 0;

    if (isFirstChild) {

        char nameFile[256] = "";

        while (true) {
            ret = read (proxyClient.m_fdRead, nameFile, 256);
            CHECK_ERROR (ret);
            if (ret == 0) {
                return 0;
            }

            int fdFile = open (nameFile, O_RDONLY);
            CHECK_ERROR (fdFile);

            while (true) {
                ret = splice (fdFile,                nullptr,
                              proxyClient.m_fdWrite, nullptr,
                              1024 * 16, SPLICE_F_MOVE);
                CHECK_ERROR (ret);

                if (ret == 0) {
                    break;
                }
            }

            char check = '\0';
            ret = read (proxyClient.m_fdRead, &check, sizeof (check));
            CHECK_ERROR (ret);
        }

    } else {
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
