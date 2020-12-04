#include "wrrd.h"
#include <unistd.h>
#include <climits>
#include "SmartFIFO.h"
#include <poll.h>
#include <wait.h>

int writer (const char conn_str[], const char name_file[]) {
    int fd_conn = open (conn_str, O_RDONLY);

    CHECK_ERROR (fd_conn);

    pid_t pid_r = 0;
    int ret = read (fd_conn, &pid_r, sizeof (pid_r));
    CHECK_ERROR (ret);

    if (ret == 0) {
        printf ("Reader not found\n");
        close (fd_conn);
        return EXIT_FAILURE;
    }

    char reader_fifo_str[MAXLEN] = "";
    sprintf (reader_fifo_str, "/tmp/temp_%d", pid_r);
    int fd_reader = open (reader_fifo_str, O_WRONLY | O_NONBLOCK);
    CHECK_ERROR (fd_reader);
    CHECK_ERROR (SetBlock (fd_reader));

    int _temp = '_EDA';                                 // For fifo
    ret = write (fd_reader, &_temp, sizeof (_temp));
    CHECK_ERROR (ret);

    int fd_file = open (name_file, O_RDONLY);
    CHECK_ERROR (fd_file);

    char buf[PIPE_BUF] = {0};
    // Начло к.с. между функцией read этого процесса и любой другой функцией другого,
    // которая работает с этим файлом на запись.
    while ((ret = read (fd_file, buf, PIPE_BUF))) {
        CHECK_ERROR (write (fd_reader, buf, ret));  // тот же FIFO
    }
    // Конец к.с.

    close (3, fd_conn, fd_reader, fd_file);

    return EXIT_SUCCESS;
}

int reader (const char conn_str[]) {

    const pid_t mypid = getpid ();

    char pers_fifo_str[MAXLEN] = "";
    sprintf (pers_fifo_str, "/tmp/temp_%d", mypid);
    CHECK_ERROR (CreateFIFO (pers_fifo_str));

    int fd_pers = open (pers_fifo_str, O_RDONLY | O_NONBLOCK);
    CHECK_ERROR (fd_pers);
    CHECK_ERROR (SetBlock (fd_pers));

    int fd_conn = open (conn_str, O_WRONLY);
    CHECK_ERROR (fd_conn);

    int ret = write (fd_conn, &mypid, sizeof (mypid));
    CHECK_ERROR (ret);

    const int time = 5;

    pollfd _poll = {0};
    _poll.fd = fd_pers;
    _poll.events = POLLIN;
    ret = poll (&_poll, 1, time * 1000);
    CHECK_ERROR (ret);

    if (ret == 0) {
        printf ("Writer not found\n");
        close (2, fd_conn, fd_pers);
        return EXIT_FAILURE;
    }

    int _temp = 0;                                      // For fifo
    ret = read (fd_pers, &_temp, sizeof (_temp));
    CHECK_ERROR (ret);

    char buf[PIPE_BUF] = "";
    while ((ret = read (fd_pers, buf, PIPE_BUF))) {
        CHECK_ERROR (fwrite (buf, ret, 1, stdout));
    }

    close (2, fd_conn, fd_pers);
    
    return EXIT_SUCCESS;
}