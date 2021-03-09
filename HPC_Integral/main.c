#include <stdio.h>
#include <math.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

#include "lib/hpc.h"
#include "lib/cpu_topology.h"

double func (double x) {
    return sin (x);
}

void* thread_print (void* num) {
    pthread_setcancelstate (PTHREAD_CANCEL_DISABLE, NULL);
    printf ("%d\n", *(int*) num);

    double* val = (double*) calloc (1, sizeof (double));
    *val = 5;

    pthread_exit (val);
}

#define handle_error_en(en, msg)\
    do {                        \
        errno = en;             \
        perror (msg);           \
        return -1;              \
    } while (0);

#define _GNU_SOURCE
#include <sched.h>

int main () {

    GetTopologyCPU ();
    //printf ("%s", __PRETTY_FUNCTION__);
    //printf ("%g\n", Integral (0, 3.141592, sin, 8));
}