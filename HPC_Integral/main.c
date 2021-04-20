#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#define DEBUG
#include "lib/hpc.h"

double func (double x) {
    return x;
}

#include <pthread.h>
#include <signal.h>

int main (int argc, char* argv[]) {
    if (argc != 2) {
        printf ("Please enter the number thread. F.e.: ./main.out 4\n");
        return -1;
    }

    const int num_threads = atoi (argv[1]);
    if (num_threads <= 0) {
        printf ("Please, enter the positive number thread\n");
        return -1;
    }

    errno = 0;
    
    double res = 0;
    hpc_int_arg_t int_arg = {
        .a = 0,
        .b = 90,
        .func = func,
        .num_threads = num_threads
    };
    res += hpcIntegral (&int_arg);
    if (errno != 0) {
        perror ("hpcIntegral");
        return -1;
    }

    printf ("%g\n", res);
}
