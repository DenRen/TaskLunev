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

int main () {
    printf ("%g\n", Integral (0, 3.141592, sin, 8));

    return 0;

    int state = 0;
    pthread_t tid = 0;

    pthread_attr_t tattr = {};
    pthread_attr_init (&tattr);

    int num = 4;
    if ((state = pthread_create (&tid, NULL, thread_print, &num)))
        handle_error_en (state, "pthread_create");

    if ((state = pthread_cancel (tid)))
        handle_error_en (state, "pthread_cancel")

    double *result = NULL;
    if ((state = pthread_join (tid, (void**) &result)))
        handle_error_en (state, "pthread_join");

    if (result != PTHREAD_CANCELED)
        printf ("%g\n", *result);
    else
        printf ("The thread %ld will be canceled\n", tid);
}