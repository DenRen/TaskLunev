#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <unistd.h>
#include <cmath>
#include <cstring>
#include <poll.h>

#include "AddFunc.h"

struct channel_t {
    int fd_read;
    int fd_write;
};

const int timeConnect = 1000;

int main (int argc, char *argv[]) {
    if (argc != 2) {
        printf ("Error input. Please enter: numberChild nameFile\n");
        return 0;
    }

    errno = 0;
    const int n = strtol ((const char *) argv, nullptr, 10);
    if (errno || n <= 0) {
        printf ("Error input. Please enter: numberChild > 0 nameFile\n");
        return 0;
    }

    channel_t channels[n];
    memset (channels, 0, sizeof (channels));

    pid_t pid = 0;
    int size_buf = 0, ret = 0;
    int pipe_sc[2] = {}, pipe_cs[2] = {};;   // Server->Client, Client->Server

    CHECK_ERROR (pipe (pipe_cs));

    pid = fork ();
    CHECK_ERROR (pid);

    if (pid == 0) {     // Child
        close (pipe_cs[0]);

        int temp = 0;   // For FIFO
        ret = write (pipe_cs[1], &temp, sizeof (temp));
        CHECK_ERROR (ret);


    } else {            // Parent
        close (pipe_cs[1]);

        pollfd checkPoll = {};
        checkPoll.fd = pipe_cs[0];
        checkPoll.events = POLLIN;
        ret = poll (&checkPoll, 1, timeConnect);
        CHECK_ERROR (ret);

        if (ret == 0) { // Time is out
            perror ("poll");
            printf ("Failed to connect to the first child!\n");

            close (pipe_cs[0]);
            return 0;
        }

        int temp = 0;   // For FIFO
        ret = read (pipe_cs[0], &temp, sizeof (temp));
        CHECK_ERROR (ret);


    }




    for (int i = 1; i < n; i++) {
        CHECK_ERROR (pipe (pipe_sc) || pipe (pipe_cs));

        channels[i].fd_read = pipe_cs[0];
        channels[i].fd_write = pipe_sc[1];

        pid = fork ();
        CHECK_ERROR (pid);

        if (pid == 0) {     // Child
            close (pipe_sc[1]);
            close (pipe_cs[0]);

            size_buf = pow (3, n - i) * 1000;

        } else {            // Parent
            close (pipe_sc[0]);
            close (pipe_cs[1]);

        }
    }
}