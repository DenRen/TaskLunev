#include "hpc.h"
#include "cpu_topology.h"
#include "debug_func.h"

#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <math.h>
#include <stdio.h>

#define _GNU_SOURCE
#include <sched.h>

const double eps = 1e-5 / 3;

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
static void* _pthread_calc_integral (void* pointer_int_arg) {
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

static void _detachThreads (pthread_t arr_tid[], size_t num_arr) {
    for (int i = 0; i < num_arr; ++i)
        pthread_detach (arr_tid[i]);
}

// tid_arr != NULL && cputop != NULL
int _distributeAttrThreads (pthread_attr_t tid_attr_arr[], int num_threads, cpu_topology_t* cputop) {
    CHECK_PTR (cputop);
    CHECK_PTR (tid_attr_arr);
    
    cpu_set_t cpu_set = {0};

    const int num_logic_cpu = cputopGetNumLogicCPU (cputop);

    CPU_ZERO (&cpu_set);
    for (int i = 0; i < num_logic_cpu; ++i)
        CPU_CLR (i, &cpu_set);

    for (int num_thread = 0; num_thread < num_threads; ++num_thread) {

        const int logic_cpu_id = cputopGetLogicCpuId (cputop, num_thread % num_logic_cpu);

        CPU_SET (logic_cpu_id, &cpu_set);

        if (pthread_attr_init (&tid_attr_arr[num_thread])) {
            PRINT_ERROR ("pthread_attr_init");
            return -1;
        }

        if (pthread_attr_setaffinity_np (&tid_attr_arr[num_thread], sizeof (cpu_set), &cpu_set)) {
            PRINT_ERROR ("pthread_setaffinity_np");
            return -1;
        }

        CPU_CLR (logic_cpu_id, &cpu_set);
    }

    return 0;
}

int _destroyAttrThread (pthread_attr_t pid_attr_arr[], unsigned num_arr) {
    CHECK_PTR (pid_attr_arr);
    for (int i = 0; i < num_arr; ++i)
        pthread_attr_destroy (&pid_attr_arr[i]);

    return 0;
}

// If error, that return NAN and errno != 0
double _integral (double a, double b, double (* func) (double),
                  int num_threads, cpu_topology_t* cputop) {
    errno = 0;
    bool sign_int = a <= b;

    if (!sign_int)
        swap_double (&a, &b);

    const double dx  = (b - a) * eps;
    const double len = (b - a) / num_threads;
    
    double         res_arr[num_threads];
    pthread_t      tid_arr[num_threads];
    pthread_attr_t tid_attr_arr[num_threads];
    integral_arg_t int_args[num_threads];

    int state = 0;

    // Distribute threads
    state = _distributeAttrThreads (tid_attr_arr, num_threads, cputop);
    if (state) {
        PRINT_ERROR ("_distributeAttrThreads");
        return NAN;
    }
    
    for (int i = 0; i < num_threads; ++i) {
        integral_arg_t* int_arg = &int_args[i];
        int_arg->dx   = dx;
        int_arg->func = func;
        int_arg->a    = a;
        int_arg->b    = a += len;

        state = pthread_create (&tid_arr[i], &tid_attr_arr[i], _pthread_calc_integral, (void*) int_arg);
        if (state) {
            PRINT_ERROR ("pthread_create");
            _detachThreads (tid_arr, i);
            return NAN;
        }
    }
    
    double res = 0;
    for (int i = 0; i < num_threads; ++i) {
        double* piece_sum = NULL;
        
        state = pthread_join (tid_arr[i], (void**) &piece_sum);
        if (state || (void*) piece_sum == NULL) {
            PRINT_ERROR ("pthread_join");
            _detachThreads (tid_arr + i + 1, num_threads - i - 1);
            _destroyAttrThread (tid_attr_arr, num_threads);
            return NAN;
        }

        res += *piece_sum;
    }

    if (_destroyAttrThread (tid_attr_arr, num_threads) == -1) {
        PRINT_ERROR ("_destroyAttrThread");
        return -1;
    }

    return res * (2 * sign_int - 1) * dx;
}

double Integral (double a, double b, double (* func) (double), int num_threads) {
    cpu_topology_t* cputop = cputopCreate ();
    if (cputop == NULL) {
        PRINT_ERROR ("cputopCreate");
        return NAN;
    }
    
    cputopInit (cputop);
    cputopSortUniqSetsCoreId (cputop);
    
    double result = _integral (a, b, func, num_threads, cputop);

    cputopDestroy (cputop);

    return result;
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