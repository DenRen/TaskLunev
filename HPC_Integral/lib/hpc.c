#include "hpc.h"

#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <math.h>
#include <stdio.h>

#define _GNU_SOURCE
#include <sched.h>

const double eps = 1e-9 / 3;

static void swap_double (double* first, double* second) {
    double temp = *first;
    *first = *second;
    *second = temp;
}

typedef struct {
    double a, b, dx;
    double (* func) (double x);
} integral_arg_t;

static bool _verifier_int_arg (const integral_arg_t int_arg) {
    return (int_arg.a    <= int_arg.b) &&
           (int_arg.dx   >= 0)         &&
           (int_arg.func != NULL);
}

// a <= b
static void* _calc_integral (void* pointer_int_arg) {
    integral_arg_t int_arg = * (integral_arg_t*) pointer_int_arg;

    if (_verifier_int_arg (int_arg) == false) {
        errno = EINVAL;
        pthread_exit (NULL);
    }
    
    double* res = (double*) calloc (1, sizeof (double));
    while (int_arg.a < int_arg.b) {
        *res += int_arg.func (int_arg.a);
        int_arg.a += int_arg.dx;
    }

    pthread_exit (res);
}

static void _detach_threads (pthread_t arr_tid[], size_t num_arr) {
    for (int i = 0; i < num_arr; ++i)
        pthread_detach (arr_tid[i]);
}

// If error, that return NAN and errno != 0
double Integral (double a, double b, double (* func) (double), int num_threads) {
    errno = 0;
    bool sign_int = a <= b;

    if (!sign_int)
        swap_double (&a, &b);

    const double dx  = (b - a) * eps;
    const double len = (b - a) / num_threads;
    
    double         res_arr [num_threads];
    pthread_t      tid_arr [num_threads];
    integral_arg_t int_args[num_threads];

    int state = 0;
    pthread_attr_t attr = {};
    if ((state = pthread_attr_init (&attr))) {
        perror ("pthread_attr_init");
        return NAN;
    }
    
    for (int i = 0; i < num_threads; ++i) {
        integral_arg_t* int_arg = &int_args[i];
        int_arg->dx   = dx;
        int_arg->func = func;
        int_arg->a    = a;
        int_arg->b    = a += len;

        state = pthread_create (&tid_arr[i], &attr, _calc_integral, (void*) int_arg);
        if (state) {
            perror ("pthread_create");
            _detach_threads (tid_arr, i);
            return NAN;
        }
    }
    
    /*
    cpu_set_t cpu_set = {};

    CPU_ZERO (&cpu_set);
    for (int i = 0; i < 8; ++i)
        CPU_CLR (i, &cpu_set);

    for (int i = 0; i < num_threads; ++i) {
        CPU_SET (i % 10, &cpu_set);

        state = pthread_setaffinity_np (tid_arr[i], sizeof (cpu_set), &cpu_set);
        if (state) {
            perror ("pthread_setaffinity_np");
            _detach_threads (tid_arr, num_threads);
            return NAN;
        }

        CPU_CLR (i % 10, &cpu_set);
    }*/

    double res = 0;
    for (int i = 0; i < num_threads; ++i) {
        double* piece_sum = NULL;
        
        state = pthread_join (tid_arr[i], (void**) &piece_sum);
        if (state || (void*) piece_sum == NULL) {
            perror ("pthread_join");
            _detach_threads (tid_arr + i + 1, num_threads - i - 1);
            return NAN;
        }

        res += *piece_sum;
    }

    return res * (2 * sign_int - 1) * dx;
}

double SimpleIntegral (double a, double b, double (* func) (double)) {
    bool sign_int = a <= b;

    if (!sign_int)
        swap_double (&a, &b);

    const double dx = (b - a) * eps;
    double res = 0;

    while (a < b) {
        res += func (a);
        a += dx;
    }

    return res * (2 * sign_int - 1) * dx;
}