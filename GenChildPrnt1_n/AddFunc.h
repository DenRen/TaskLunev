#ifndef TASK2FD_ADDFUNC_H
#define TASK2FD_ADDFUNC_H

#include <cstdlib>
#include <cerrno>

void NOP ();

void close (int count, int first, ...);

#define CHECK_ERROR(ret)                    \
    if (ret == -1) {                        \
        printf ("LINE:  %d\n", __LINE__);   \
        printf ("errno: %d\n", errno);      \
        perror ("");                        \
        return -1;                          \
    } else {                                \
        errno = 0;                          \
    } NOP ()

#define CHECK_TRUE(val)                     \
    if (val == false) {                     \
        printf ("LINE: %d\n", __LINE__);    \
        printf ("errno: %d\n", errno);      \
        perror ("");                        \
        return -1;                          \
    } else {                                \
        errno = 0;                          \
    } NOP ()

#endif //TASK2FD_ADDFUNC_H
