#include <poll.h>
#include "SmartFIFO.h"
#include "semaphore.h"

/*
int main (int argc, char *argv[]) {
    // Get params -----------------------------------------------------------------------

    long key = GetKeyWithConsole (argc, argv);

    // Start writer_2 or reader_2 -----------------------------------------------------------

    const char conn_1_str[] = "connFIFO_1", conn_2_str[] = "connFIFO_2";
    const char name_fifo_w[] = "temp_fifo_w", name_fifo_r[] = "temp_fifo_r";

    CHECK_RET_FUNC (CreateFIFO (name_fifo_w));
    CHECK_RET_FUNC (CreateFIFO (name_fifo_r));

    int ret = 0, fd = 0;

    if (key == KEY::WRITER) {
        printf ("Writer started\n");

        int _temp = 0;
        fd = open (name_fifo_w, O_RDONLY | O_NONBLOCK);
        check_error (fd, "open", 0);
        while (true) {
            ret = read (fd, &_temp, sizeof (_temp));

            if (ret == -1) {
                if (errno != EAGAIN)
                    check_error (ret, "read", 0);
            } else if (ret != 0) {
                printf ("%d\n", _temp);
            }
        }

        printf ("Writer finished");

    } else if (key == KEY::READER) {
        printf ("Reader started\n");

        fd = open (name_fifo_w, O_WRONLY | O_NONBLOCK);
        check_error (fd, "open", 0);

        int _temp = 7;
        ret = write (fd, &_temp, sizeof (_temp));
        check_error (ret, "write", 0);
        printf ("ret: %d\n", ret);

        printf ("Reader finished\n");
    }

    exit (EXIT_SUCCESS);
}*/

int main (int argc, char *argv[]) {
    // Get params -----------------------------------------------------------------------

    long key = GetKeyWithConsole (argc, argv);

    // Start writer_2 or reader_2 -----------------------------------------------------------

    const char conn_1_str[] = "connFIFO_1", conn_2_str[] = "connFIFO_2";
    CHECK_RET_FUNC (CreateFIFO (conn_1_str));
    CHECK_RET_FUNC (CreateFIFO (conn_2_str));

    const pid_t mypid = getpid ();
    CHECK_ERROR (mypid, getpid);

    if (key == KEY::WRITER)
        return writer (conn_1_str, conn_2_str);
    else if (key == KEY::READER)
        return reader (conn_1_str, conn_2_str);

    exit (EXIT_SUCCESS);
}

/*
int main (int argc, char *argv[]) {
    // Get params -----------------------------------------------------------------------

    long key = GetKeyWithConsole (argc, argv);

    // Start writer_2 or reader_2 -----------------------------------------------------------

    const char name_Conn_FIFO[] = "nameBaseFIFO";

    if (mkfifo (name_Conn_FIFO, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH) == -1 && errno != EEXIST) {
        perror ("mkfifo");
        return EXIT_FAILURE;
    }

    if (key == KEY::WRITER) {
        printf ("Writer\n");

        int fd = open (name_Conn_FIFO, O_WRONLY | O_NONBLOCK);
        if (fd == -1) {
            if (errno == ENXIO) {
                printf ("FIFO don't open on read!\n");
            } else {
                perror ("open");
                return EXIT_FAILURE;
            }
        }
        printf ("FIFO opened\n");
        close (fd);
        printf ("FIFO closed\n");
    } else {
        printf ("Reader\n");
        sleep (3);
        int fd = open (name_Conn_FIFO, O_RDONLY | O_NONBLOCK);
        if (fd == -1) {
            perror ("open");
            return EXIT_FAILURE;
        }
        printf ("FIFO opened\n");

        pollfd _poll;
        _poll.fd = fd;
        _poll.events = POLLHUP;

        sleep (6);
        const int time_delay = 6; // Seconds
        printf ("Start check\n");
        int ret = poll (&_poll, 1, time_delay * 1000);

        if (ret == -1) {
            close (fd);
            perror ("poll");
            return EXIT_FAILURE;
        } else if (ret == 0) {
            printf ("Событий не произошло\n");
        } else {
            printf ("revents: %d\n", _poll.revents);
            if (_poll.revents & POLLHUP) {
                _poll.revents = 0;
                printf ("POLLHUP\n");
            }
        }

        close (fd);
    }

    printf ("End\n");

    exit (EXIT_SUCCESS);
}
*/

#define P(sem) mysem_wait (sem);
#define V(sem) mysem_post (sem);
/*
int main (int argc, char *argv[]) {
    // Get params -----------------------------------------------------------------------

    long key = GetKeyWithConsole (argc, argv);

    // Start writer_2 or reader_2 -----------------------------------------------------------

    const char name_Conn_FIFO[] = "nameBaseFIFO";

    //if (mkfifo (name_Conn_FIFO, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH) == -1 && errno != EEXIST) {
    //    perror ("mkfifo");
    //    return EXIT_FAILURE;
    //}

    const char name_mut_ex[] = "mut_ex";
    const char name_full[]   = "full";
    const char name_empty[]  = "empty";

    mysem_t *mut_ex = mysem_open (name_mut_ex, O_CREAT, 0666, 1);
    mysem_t *full   = mysem_open (name_full,   O_CREAT, 0666, 0);
    mysem_t *empty  = mysem_open (name_empty,  O_CREAT, 0666, 1);

    if (key == KEY::WRITER) {
        printf ("Writer\n");

        P (empty);
        P (mut_ex);
        sleep (1);
        printf ("Eeeee\n");
        V (mut_ex);
        V (full);

    } else {
        printf ("Reader\n");

        P (full);
        P (mut_ex);
        sleep (1);
        printf ("Eeeee\n");
        V (mut_ex);
        V (empty);

    }

    mysem_close (empty);
    mysem_close (full);
    mysem_close (mut_ex);

    mysem_unlink (name_empty);
    mysem_unlink (name_full);
    mysem_unlink (name_mut_ex);

    printf ("End\n");

    exit (EXIT_SUCCESS);
} */

/*
int main (int argc, char *argv[]) {
    // Get params -----------------------------------------------------------------------

    long key = GetKeyWithConsole (argc, argv);

    // Start writer_2 or reader_2 -----------------------------------------------------------

    const char name_Conn_FIFO[] = "nameBaseFIFO";

    if (mkfifo (name_Conn_FIFO, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH) == -1 && errno != EEXIST) {
        perror ("mkfifo");
        return EXIT_FAILURE;
    }

    signal_pipe ();

    if (key == KEY::WRITER) {
        printf ("Writer\n");

        int fd = open (name_Conn_FIFO, O_WRONLY);
        if (fd == -1) {
            perror ("open");
            return EXIT_FAILURE;
        }

        for (int i = 0; ; i++) {
            printf ("I work %d\n", i);

            int num = 5;
            if (write (fd, &num, sizeof (int)) < 0) {
                perror ("write");
                return EXIT_FAILURE;
            }

            sleep (1);
        }

    } else {
        printf ("Reader\n");
        int fd = open (name_Conn_FIFO, O_RDONLY);
        if (fd == -1) {
            perror ("open");
            return EXIT_FAILURE;
        }
        
        for (int i = 0; ; i++) {

            int buf = 0, size = 0;
            if ((size = read (fd, &buf, sizeof (int))) < 0) {
                perror ("read");
                return EXIT_FAILURE;
            }

            printf ("I read. Size: %d, buf: %d, errno: %d\n", size, buf, errno);

            sleep (1);
        }
    }

    exit (EXIT_SUCCESS);
}*/

/*
int main (int argc, char *argv[]) {
    // Get params -----------------------------------------------------------------------
    if (argc != 2) {
        printf ("Please? enter %s <%d/%d>, when %d - writer, %d - reader\n",
                argv[0], KEY::WRITER, KEY::READER, KEY::WRITER, KEY::READER);
        exit (EXIT_FAILURE);
    }

    long key = str2long (argv[1]);
    if (errno != KEY::WRITER) {
        perror ("str2long");
        exit (EXIT_FAILURE);
    }

    if (key != KEY::WRITER && key != KEY::READER) {
        printf ("Please enter %d (writer) or %d (reader)\n", KEY::WRITER, KEY::READER);
        exit (EXIT_FAILURE);
    }

    // Start writer or reader -----------------------------------------------------------

    const char name_Conn_FIFO[] = "nameBaseFIFO";

    if (initFIFO (name_Conn_FIFO) == -1) {
        perror ("initFIFO");
        exit (EXIT_FAILURE);
    }

    const size_t size_data = 1024 * 16;

    if (key == KEY::WRITER) {

        // Generate data ------------------------------------------------

        size_t *data = (size_t *) calloc (size_data, sizeof (size_t));
        for (size_t i = 0; i < size_data; i++)
            data[i] = i;

        // Send data ----------------------------------------------------

        printf ("Writer started\n");
        fflush (stdout);
        if (writer (name_Conn_FIFO, (char *) data, size_data * sizeof (*data)) == -1) {
            free (data);

            perror ("writer_2");
            exit (EXIT_FAILURE);
        }

        free (data);

        printf ("Write successful --------------------> GOOD\n");
    } else {

        // Read data ----------------------------------------------------

        size_t *data = nullptr;
        size_buf_t size_buf = 0;

        printf ("Reader started\n");
        fflush (stdout);
        if (reader (name_Conn_FIFO, (char **) &data, &size_buf) == -1) {
            perror ("reader_2");
            exit (EXIT_FAILURE);
        }

        bool _error = false;
        if (size_buf != size_data * sizeof (*data))
            printf ("Failed read:\nsize_buf  = %zx\nsize_data = %zu\n ---------> FAILED", size_buf, size_data);
        else {

            for (size_t i = 0; i < size_data / sizeof (*data); i++)
                if (data[i] != i) {
                    _error = true;
                    printf ("Incorrect read data --------------------> FAILED\n");
                    break;
                }
        }

        if (_error == false)
            printf ("Read successful --------------------> GOOD\n");
        free (data);
    }

    exit (EXIT_SUCCESS);
}*/

// TODO Template main
/*
int main (int argc, char *argv[]) {
    // Get params -----------------------------------------------------------------------

    long key = GetKeyWithConsole (argc, argv);

    // Start writer_2 or reader_2 -----------------------------------------------------------

    const char conn_1_str[] = "connFIFO_1", conn_2_str[] = "connFIFO_2";


    if (key == KEY::WRITER) {
        printf ("Writer started\n");

        printf ("Writer finished");

    } else if (key == KEY::READER) {

        printf ("Reader started\n");

        printf ("Reader finished\n");
    }

    exit (EXIT_SUCCESS);
}
*/