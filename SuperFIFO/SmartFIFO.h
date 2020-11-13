#ifndef SUPERFIFO_SMARTFIFO_H
#define SUPERFIFO_SMARTFIFO_H

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cerrno>
#include <random>

#define CHECK_ERROR(ret)                    \
    if (ret == -1) {                        \
        printf ("LINE: %d\n", __LINE__);    \
        return EXIT_FAILURE;                \
    } else {                                \
        errno = 0;                          \
    } NOP ()

#define CHECK CHECK_RET_FUNC (check_error

#define CHECK_RET_FUNC(ret)             \
    if (ret != EXIT_SUCCESS) {          \
        printf ("LINE: %d\n", __LINE__);\
        return EXIT_FAILURE;            \
    } NOP ()

void NOP ();

#define MAXLEN 64

enum KEY {
    WRITER,
    READER
};

typedef unsigned long rand_num_t;
typedef size_t size_buf_t;

int CreateFIFO (const char *nameFIFO, int mask = 0666);
int writer_2 (const char *name_Conn_FIFO, char *data, size_buf_t size);
int reader_2 (const char *name_Conn_FIFO, char **buf, size_buf_t *size);

long GetKeyWithConsole (int argc, char *argv[]);

long str2long (const char *str);

struct header_message_t {
    pid_t sdr_pid;  // Sender pid
    pid_t rcvr_pid; // Receiver pid
    short size;
};

static sigset_t signal_new, signal_old, signal_leer;
void signal_pipe ();

int check_error (int ret, const char name[], int count = 0,  ...);

struct msg_t {
    pid_t sender;
    pid_t receiver;
};

bool FileIsOpenOnWrite (const char *nameFile);

int SendMsg (int fd, void *data, int size, bool block = false);

int reader_3 (const char *conn_1_str, const char *conn_2_str);
int writer_3 (const char *conn_1_str, const char *conn_2_str);

int SetFlag (int fd, unsigned flag);
int RemoveFlag (int fd, unsigned flag);

int SetBlock (int fd);
int SetNonBlock (int fd);

void close (int count, int first, ...);

#endif //SUPERFIFO_SMARTFIFO_H
