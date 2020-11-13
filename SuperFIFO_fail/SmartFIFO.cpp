#include <climits>
#include <signal.h>
#include <cstdarg>
#include <wait.h>
#include "SmartFIFO.h"

void NOP () { return; }

int CreateFIFO (const char *nameFIFO, int mask) {
    if (mkfifo (nameFIFO, mask) == -1) {
        if (errno == EEXIST) {
            errno = 0;
            return EXIT_SUCCESS;
        } else {
            perror ("mkfifo");
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}
/*
int reader_2 (const char *name_Conn_FIFO, char **buf, size_buf_t *size) {

    int fd_conn = open (name_Conn_FIFO, O_WRONLY);
    if (fd_conn == -1) {
        perror ("open (conn)");
        return -1;
    }

    printf ("Connect FIFO opened\n");

    // Generate random name fifo file -------------------------------

    std::random_device rd;
    std::mt19937_64 mersenne (rd ());

    const char max_len_name = 64;
    char nameFIFO[max_len_name] = "";

    rand_num_t rand_num = 0;
    while (true) {
        rand_num = mersenne ();
        sprintf (nameFIFO, "temp_%ld", rand_num);

        if (mkfifo (nameFIFO, 0777) == -1) {
            if (errno == EEXIST)
                continue;
            else {
                close (fd_conn);

                perror ("mkfifo (local)");
                return -1;
            }
        }

        break;
    }

    printf ("Created local FIFO file %s\n", nameFIFO);

    // Second send nameFIFO

    if (write (fd_conn, &rand_num, sizeof (rand_num)) == -1) {
        close (fd_conn);

        perror ("write conn_msg");
        return -1;
    }

    printf ("rund_num sent\n");

    // First connect to FIFO

    int fd = open (nameFIFO, O_RDONLY | O_NONBLOCK);
    if (fd == -1) {
        close (fd_conn);

        perror ("open (data)");
        return -1;
    }
    errno = 0;

    printf ("Local file open\n");

    // Read and wait answer with fifo -------------------------------

    if (read (fd, size, sizeof (size_buf_t)) != sizeof (size_buf_t)) {
        close (fd_conn);
        close (fd);

        perror ("read (size)");
        return -1;
    }

    printf ("size_data: %zu\n", *size);

    *buf = (char *) calloc (*size, sizeof (char));
    if (buf == nullptr) {
        close (fd_conn);
        close (fd);

        perror ("calloc");
        return -1;
    }

    *buf = buf;

    size_buf_t cur_size = *size;
    const int quant_pages = cur_size / PIPE_BUF + (cur_size % PIPE_BUF != 0);
    int size_page = 0;

    for (int i = 0; i < quant_pages; i++) {
        if (cur_size - PIPE_BUF < 0)
            size_page = cur_size;
        else
            size_page = PIPE_BUF;
        cur_size -= PIPE_BUF;

        int rnum = read (fd, buf, size_page);
        if (rnum == -1) {
            close (fd);

            perror ("read (data)");
            return -1;
        } else if (rnum == 0) {
            close (fd);

            perror ("Writer disconnect!");
            return -1;
        }
        printf (".");
        buf += size_page;
    }
    printf ("\n");
    close (fd_conn);
    close (fd);

    return 0;
}
*/
long str2long (const char *str) {
    assert (str != nullptr);

    const int base = 10;
    char *error = nullptr;

    long num = strtol (str, &error, base);

    if (errno != 0 || *error)
        perror ("strtol");

    return num;
}

static void sigfunc (int sig_nr) {
    printf ("Поймали SIGPIPE!\n");
    return;
}

void signal_pipe () {
    if (signal (SIGPIPE, sigfunc) == SIG_ERR) {
        printf ("Невозможно получить сигнал SIGPIPE\n");
        return;
    }

    /*Удаляем все сигналы из множества сигналов*/
    sigemptyset(&signal_leer);
    sigemptyset(&signal_new);
    sigaddset(&signal_new, SIGPIPE);
    /*Устанавливаем signal_new и сохраняем его*/

    /* теперь маской сигналов будет signal_old*/
    if(sigprocmask(SIG_UNBLOCK, &signal_new, &signal_old) < 0)
        return;

}

int check_error (int ret, const char name[], int count,  ...) {
    if (ret == -1) {
        printf ("errno: %d\n", errno);
        perror (name);
        if (count > 0) {
            va_list ap;     // Argument processing
            va_start (ap, count);
            for (int i = 0; i < count; i++)
                close (va_arg (ap, int));
        }

        return EXIT_FAILURE;
    }
    errno = 0;

    return EXIT_SUCCESS;
}

bool FileIsOpenOnWrite (const char *nameFile) {

    int fd = open (nameFile, O_WRONLY | O_NONBLOCK);
    if (fd == -1)
        return false;

    close (fd);
    return true;

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

int SendMsg (int fd, void *data, int size, bool block) {
    assert (size >= 0);

    int pid = fork ();
    if (pid == 0) { // Child
        errno = 0;
        if (block) {
            CHECK_RET_FUNC (SetBlock (fd));
        } else {
            CHECK_RET_FUNC (SetNonBlock (fd));
        }

        int ret = write (fd, data, size);
        exit (errno);
    } else {
        int status = 0;
        pid_t ret = waitpid (pid, &status, 0);
        CHECK_ERROR (check_error (ret, "waitpid"));
        if (WIFSIGNALED (status))
            errno = ENXIO;

        return -1 * WIFSIGNALED (status);
    }
}

void close (int count, int first, ...) {
    close (first);

    va_list ap;
    va_start (ap, first);
    for (int i = 1; i < count; i++)
        close (va_arg (ap, int));
}