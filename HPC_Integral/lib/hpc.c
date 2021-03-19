#include "hpc.h"
#include "cpu_topology.h"
//#define DEBUG
#include "debug_func.h"

#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <math.h>
#include <stdio.h>

#define _POSIX_PRIORITY_SCHEDULING
#include <unistd.h>

const double eps = 1e-10;

// ============\\ 
// Main structs ---------------------------------------------------------------
// ============//

typedef struct {
    double a, b, dx;
    double (* func) (double x);
    double result;
} integral_arg_t;

// ===================\\ 
// Secondary functions ------------------------------------------------------------
// ===================//

static void swap_double (double* first, double* second) {
    double temp = *first;
    *first = *second;
    *second = temp;
}

static bool _verifier_int_arg (const integral_arg_t int_arg) {
    return (int_arg.a    <= int_arg.b) &&
           (int_arg.dx   >= 0)         &&
           (int_arg.func != NULL);
}

// =============================\\ 
// Calc integral functions [DEV] -------------------------------------------------------------
// =============================//

// a, b is number, func != NULL, num_threads > 0 and cputop != NULL
// a <= b
static void* _pthread_calc_integral (void* pointer_int_arg) {
    register integral_arg_t int_arg = *(integral_arg_t*) pointer_int_arg;

    IF_DEBUG_NON_PRINT (
        if (_verifier_int_arg (int_arg) == false) {
            errno = EINVAL;
            PRINT_ERROR ("Invalid input arguments for calculating the integral!\n"
                        "Maybe a, b is NAN");
            pthread_exit ((void*) 1);
        }
    );
/*
    if (pthread_setconcurrency (99 + pthread_self () % 2)) {
        PRINT_ERROR ("pthread_setconcurrency");
        pthread_exit ((void*) 1);
    }
*/
#ifdef VECT_CALC
    const int _num = 4;
    register double x[_num];
    x[0] = int_arg.a;
    x[1] = int_arg.a + int_arg.dx;
    x[2] = int_arg.a + 2 * int_arg.dx;
    x[3] = int_arg.a + 3 * int_arg.dx;

    register double dx = _num * int_arg.dx;

    register double res[4];
    res[0] = 0;
    res[1] = 0;
    res[2] = 0;
    res[3] = 0;
    while (x[3] < int_arg.b) {        
        res[0] += int_arg.func (x[0]);
        res[1] += int_arg.func (x[1]);
        res[2] += int_arg.func (x[2]);
        res[3] += int_arg.func (x[3]);

        x[0] += dx;
        x[1] += dx;
        x[2] += dx;
        x[3] += dx;
    }

    ((integral_arg_t*) pointer_int_arg)->result = res[0] + res[1] + res[2] + res[3];

#else

    int_arg.result = 0;
    while (int_arg.a < int_arg.b) {
        int_arg.result += int_arg.func (int_arg.a);
        int_arg.a += int_arg.dx;
    }

    ((integral_arg_t*) pointer_int_arg)->result = int_arg.result;

#endif

    return NULL;
}

static void _detachThreads (pthread_t arr_tid[], size_t num_arr) {
    for (int i = 0; i < num_arr; ++i)
        pthread_detach (arr_tid[i]);
}

static int _destroyAttrThread (pthread_attr_t pid_attr_arr[], unsigned num_arr) {
    CHECK_PTR (pid_attr_arr);
    for (int i = 0; i < num_arr; ++i)
        pthread_attr_destroy (&pid_attr_arr[i]);

    return 0;
}

// To evenly distribute the load, you should sort cputop by sorting cputopSortUniqSetsCoreId
// tid_arr != NULL && cputop != NULL
static int _distributeAttrThreads (pthread_attr_t tid_attr_arr[], unsigned num_threads, cpu_topology_t* cputop) {
    IF_DEBUG_NON_PRINT (
        CHECK_PTR (cputop);
        CHECK_PTR (tid_attr_arr);
        
        if (cputopVerifier (cputop) == false) {
            PRINT_ERROR ("cputopVerifier");
            errno = EINVAL;
            return -1;
        }

        if (num_threads == 0) {
            PRINT_ERROR ("num_threads == 0");
            errno = EINVAL;
            return -1;
        }
    );

    const int num_logic_cpu = cputopGetNumLogicCPU (cputop);
    if (num_logic_cpu == -1) {
        PRINT_ERROR ("cputopGetNumLogicCPU");
        return -1;
    }

    cpu_set_t cpuset = {0};

    for (unsigned num_thread = 0; num_thread < num_threads; ++num_thread) {

        pthread_attr_t* tid_attr = &tid_attr_arr[num_thread];

        if (pthread_attr_init (tid_attr)) {
            PRINT_ERROR ("pthread_attr_init");
            return -1;
        }

        const int logic_cpu_id = cputopGetLogicCpuId (cputop, num_thread % num_logic_cpu);
        if (logic_cpu_id == -1) {
            PRINT_ERROR ("cputopGetLogicCpuId");
            return -1;
        }

        IF_DEBUG (printf ("Set logic cpu id: %d\n", logic_cpu_id));

        CPU_ZERO (&cpuset);
        CPU_SET (logic_cpu_id, &cpuset);

        if (pthread_attr_setaffinity_np (tid_attr, sizeof (cpuset), &cpuset)) {
            PRINT_ERROR ("pthread_setaffinity_np");
            return -1;
        }
    }

    return 0;
}

// If error, that return NAN and errno != 0
// a, b is number, func != NULL, num_threads > 0 and cputop != NULL
static double _integral (double a, double b, double (* func) (double),
                         const int num_threads, cpu_topology_t* cputop) {
    
    IF_DEBUG_NON_PRINT (
        if ((isfinite (a) && isfinite (b) && func != NULL && num_threads > 0) == false) {
            errno = EINVAL;
            PRINT_ERROR ("(isfinite (a) && isfinite (b) && func == NULL && num_threads > 0) is false");
            return NAN;
        }
        CHECK_PTR (cputop);
        if (cputopVerifier (cputop) == false) {
            errno = EINVAL;
            PRINT_ERROR ("cputop is not init: cputopVerifier (cputop) == false");
            return NAN;
        }
    );

    // Distribute threads to core and hyperthreads
    pthread_attr_t tid_attr_arr[num_threads];
    int state = _distributeAttrThreads (tid_attr_arr, num_threads, cputop);
    if (state) {
        PRINT_ERROR ("_distributeAttrThreads");
        return NAN;
    }


    // Set a < b
    bool sign_int = a <= b;
    if (!sign_int)
        swap_double (&a, &b);

    const double dx  = (b - a) * eps;
    const double len = (b - a) / num_threads;

    pthread_t      tid_arr [num_threads];
    integral_arg_t int_args[num_threads];
    
    // Create pthreads with special attributs
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
            _destroyAttrThread (tid_attr_arr, num_threads);
            return NAN;
        }
    }
    
    if (_destroyAttrThread (tid_attr_arr, num_threads) == -1) {
        PRINT_ERROR ("_destroyAttrThread");
        _detachThreads (tid_arr, num_threads);
        return -1;
    }

    // Join all pthreads and check for error by ret_val
    double res = 0;
    for (int i = 0; i < num_threads; ++i) {
        int* ret_val = NULL;
        state = pthread_join (tid_arr[i], (void**) &ret_val);
        
        if (state || ret_val != NULL) {
            errno = EINVAL;
            PRINT_ERROR ("pthread_join");
            _detachThreads (tid_arr + i + 1, num_threads - i - 1);
            return NAN;
        }
        
        res += int_args[i].result;
    }

    return res * (2 * sign_int - 1) * dx;
}

// If error, that return NAN and errno != 0
// a, b is number, func != NULL, num_threads > 0 and cputop != NULL
static double _integral_linear (double a, double b, double (* func) (double),
                                int num_threads, cpu_topology_t* cputop) {
    IF_DEBUG_NON_PRINT (
        if ((isfinite (a) && isfinite (b) && func != NULL && num_threads > 0) == false) {
            errno = EINVAL;
            PRINT_ERROR ("(isfinite (a) && isfinite (b) && func == NULL && num_threads > 0) is false");
            return NAN;
        }
        CHECK_PTR (cputop);
        if (cputopVerifier (cputop) == false) {
            errno = EINVAL;
            PRINT_ERROR ("cputop is not init: cputopVerifier (cputop) == false");
            return NAN;
        }
    );
    
    int num_dummy = 0, num_logic_cpu = cputopGetNumLogicCPU (cputop);
    if (num_threads < num_logic_cpu)
        num_dummy = num_logic_cpu - num_threads;
    else if (num_threads > num_logic_cpu)
        num_threads = num_logic_cpu;

    // Distribute threads to core and hyperthreads
    pthread_attr_t tid_attr_arr[num_threads + num_dummy];
    int state = _distributeAttrThreads (tid_attr_arr, num_threads + num_dummy, cputop);
    if (state) {
        PRINT_ERROR ("_distributeAttrThreads");
        return NAN;
    }

    // Set a < b
    bool sign_int = a <= b;
    if (!sign_int)
        swap_double (&a, &b);

    const double dx  = (b - a) * eps;
    const double len = (b - a) / num_threads;

    pthread_t      tid_arr [num_threads + num_dummy];
    integral_arg_t int_args[num_threads + num_dummy];
    
    // Create pthreads with special attributs
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
            _destroyAttrThread (tid_attr_arr, num_threads + num_dummy);
            return NAN;
        }
    }
    
    // n v t = s n
    // m v t = s m

    if (num_dummy != 0) {
        const double dx_dummy  = dx  * num_threads / num_dummy;
        const double len_dummy = len * num_threads / num_dummy;

        a -= len * num_threads;

        // Create dummy pthreads with special attributs
        for (int i = num_threads; i < num_threads + num_dummy; ++i) {
            integral_arg_t* int_arg = &int_args[i];
            int_arg->dx   = dx_dummy;
            int_arg->func = func;
            int_arg->a    = a;
            int_arg->b    = a += len_dummy;
            
            state = pthread_create (&tid_arr[i], &tid_attr_arr[i], _pthread_calc_integral, (void*) int_arg);
            if (state) {
                PRINT_ERROR ("pthread_create");
                _detachThreads (tid_arr, i);
                _destroyAttrThread (tid_attr_arr, num_threads + num_dummy);
                return NAN;
            }
        }
    }

    if (_destroyAttrThread (tid_attr_arr, num_threads + num_dummy) == -1) {
        PRINT_ERROR ("_destroyAttrThread");
        _detachThreads (tid_arr, num_threads);
        return -1;
    }

    // Join all pthreads and check for error by ret_val
    double res = 0;
    for (int i = 0; i < num_threads + num_dummy; ++i) {
        int* ret_val = NULL;
        state = pthread_join (tid_arr[i], (void**) &ret_val);
        
        if (state || ret_val != NULL) {
            errno = EINVAL;
            PRINT_ERROR ("pthread_join");
            _detachThreads (tid_arr + i + 1, num_threads + num_dummy - i - 1);
            return NAN;
        }
        
        if (i < num_threads)
            res += int_args[i].result;
    }

    return res * (2 * sign_int - 1) * dx;
}

// =======================\\ 
// Calc integral functions -------------------------------------------------------------
// =======================//

double hpcIntegral (double a, double b, double (* func) (double), const unsigned num_threads) {

    if ((isfinite (a) && isfinite (b) && func != NULL && num_threads > 0) == false) {
        errno = EINVAL;
        IF_DEBUG (
            printf ("\n");
            printf ("\tisfinite (a): %d\n", isfinite (a));
            printf ("\tisfinite (b): %d\n", isfinite (b));
            printf ("\tfunc: %p\n", func);
            printf ("\tnum_threads: %d\n", num_threads)
        );
        PRINT_ERROR ("(isfinite (a) && isfinite (b) && func == NULL && num_threads > 0) is false");
        return NAN;
    }

    cpu_topology_t* cputop = cputopCreate ();
    if (cputop == NULL) {
        PRINT_ERROR ("cputopCreate");
        return NAN;
    }
    
    if (cputopInit (cputop) == -1) {
        PRINT_ERROR ("cputopInit");
        cputopDestroy (&cputop);
        return NAN;
    }
    
    if (cputopSortUniqSetsCoreId (cputop) == -1) {
        PRINT_ERROR ("cputopSortUniqSetsCoreId");
        cputopDestroy (&cputop);
        return NAN;
    }
    
    // Less than 0.01 seconds passed

#ifndef LINEAR_TIME_CALC
    double result = _integral (a, b, func, num_threads, cputop);
#else
    double result = _integral_linear (a, b, func, num_threads, cputop);
#endif

    if (cputopDestroy (&cputop) == -1) {
        PRINT_ERROR ("cputopDestroy");
        return -1;
    }

    return result;
}

double hpcSimpleIntegral (double a, double b, double (* func) (double)) {
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

// ==================\\ 
// Priority functions -------------------------------------------------------------
// ==================//

static int _printPolicy (int pid) {
    int policy = sched_getscheduler (pid);
    if (policy == -1) {
        perror ("sched_getscheduler");
        return -1;
    }

    switch (policy) {
        case SCHED_OTHER:    printf ("policy: OTHER\n");    break;
        case SCHED_FIFO:     printf ("policy: FIFO\n");     break;
        case SCHED_RR:       printf ("policy: RR\n");       break;
        case SCHED_BATCH:    printf ("policy: BATCH\n");    break;
        case SCHED_IDLE:     printf ("policy: IDLE\n");     break;
        case SCHED_ISO:      printf ("policy: ISO\n");      break;
        case SCHED_DEADLINE: printf ("policy: DEADLINE\n"); break;
        default: {
            printf ("policy: %d\n", policy);
            perror ("policy not found");
            return -1;
        }
    }
}

int hpcSetPolicy (const int policy, unsigned priority) {
    struct sched_param sp = { .sched_priority = 99 };

    if (sched_setscheduler (0, policy, &sp) == -1) {
        perror ("sched_setscheduler");
        return -1;
    }
    
    IF_DEBUG (
        _printPolicy (getpid ());
    
        if (policy == SCHED_RR) {
            struct timespec time = {0};
            if (sched_rr_get_interval (0, &time) == -1) {
                perror ("sched_rr_get_interval");
                return -1;
            }

            printf ("rr_interval: %ld ms\n", time.tv_nsec / 1000000);
        }

        sched_getparam (0, &sp);
        printf ("priority: %d\n", sp.sched_priority);

        printf ("min %d\nmax %d\n", sched_get_priority_min (policy),
                                    sched_get_priority_max (policy));
    );
    
    return 0;
}
