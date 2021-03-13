#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include "lib/hpc.h"

double func (double x) {
    return x;
}

#include <pthread.h>

int main (int argc, char* argv[]) {
    if (argc != 2) {
        printf ("Please enter the number thread. F.e.: ./main.out 4\n");
        return -1;
    }
    
    const int num_threads = atoi (argv[1]);
    if (num_threads <= 0 || num_threads > 256) {
        printf ("Please, enter the positive number thread (< 256)\n");
        return -1;
    }

    errno = 0;

    if (pthread_setconcurrency (8) == -1) {
        perror ("pthread_setconcurrency");
        return -1;
    }

    double res = hpcIntegral (0, 90, func, num_threads);
    if (errno != 0) {
        perror ("hpcIntegral");
        return -1;
    }

    printf ("%g\n", res);
}
