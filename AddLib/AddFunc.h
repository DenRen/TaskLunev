#ifndef TASK2FD_ADDFUNC_H
#define TASK2FD_ADDFUNC_H

#include <cstdlib>
#include <cstdio>
#include <cerrno>

void NOP ();

void close (int count, int first, ...);

//#define PRINT_DEBUG
#ifdef PRINT_DEBUG
    #define _ if (true)
#else
    #define _ if (false)
#endif

#define PRINT_ERROR_MSG                     \
    {                                       \
        printf ("FILE:  %s\n", __FILE__);   \
        printf ("LINE:  %d\n", __LINE__);   \
        printf ("errno: %d\n", errno);      \
        perror ("");                        \
    }

#define RETURN_EXIT_FAILED                  \
    {                                       \
        PRINT_ERROR_MSG;                    \
        return -1;                          \
    }

#define CHECK_ERROR(ret)                    \
    if ((ret) == -1) {                      \
        RETURN_EXIT_FAILED;                 \
    } else {                                \
        errno = 0;                          \
    } NOP ()

#define CHECK_TRUE(val)                     \
    if ((val) == false) {                   \
        RETURN_EXIT_FAILED;                 \
    } else {                                \
        errno = 0;                          \
    } NOP ()

#define CHECK_NULLPTR(ret)                  \
    if ((ret) == nullptr) {                 \
        RETURN_EXIT_FAILED;                 \
    } else {                                \
        errno = 0;                          \
    } NOP ()

#endif //TASK2FD_ADDFUNC_H
