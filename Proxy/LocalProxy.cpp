#include "LocalProxy.h"

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
}*/

CloseChannels (channels, numberChild
);
//printf ("Parent finished\numberChild");
}