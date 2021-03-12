#include "hpc_new.h"

#include <pthread.h>
#include <assert.h>
#include <math.h>

#include <sched.h>
#define _GNU_SOURCE

#include <stdio.h>
const double eps = 1e-10 / 5;

typedef struct {
    double a, b, dx;
    double (* func) (double x);
    double result;
    int id;
} integral_arg_t;

static void* _pthread_int_calc (void* pointer_int_arg) {
    assert (pointer_int_arg != NULL);

    integral_arg_t int_arg = *(integral_arg_t*) pointer_int_arg;

    cpu_set_t cpuset = {0};
    CPU_ZERO (&cpuset);
    CPU_SET (int_arg.id, &cpuset);

	int i = -1;
	while (CPU_ISSET (++i, &cpuset) == 0);
	printf ("CPU: %d\n", i);

    assert (pthread_setaffinity_np (pthread_self (), sizeof (cpu_set_t), &cpuset) == 0);

    double res = 0;
    while (int_arg.a < int_arg.b) {
        res += int_arg.func (int_arg.a);
        int_arg.a += int_arg.dx;
    }

    ((integral_arg_t*) pointer_int_arg)->result = res;

    return NULL;
}

double integral (double a, double b, double (*func) (double), const unsigned num_threads) {
    assert (a < b);
    assert (num_threads > 0);

    /*cpu_set_t cpu_set = {0};
    CPU_ZERO (&cpu_set);
    for (int i = 0; i < 8; ++i)
        CPU_CLR (i % 8, &cpu_set);

    pthread_attr_t pid_attr[num_threads];
    for (int i = 0; i < num_threads; ++i) {
        assert (pthread_attr_init (&pid_attr[i]) == 0);

        CPU_SET (i % 8, &cpu_set);
        assert (pthread_attr_setaffinity_np (&pid_attr[i], sizeof (cpu_set), &cpu_set) == 0);
        CPU_CLR (i % 8, &cpu_set);
    }*/

    pthread_t pid_arr[num_threads];
    integral_arg_t int_args[num_threads];

    const double len = (b - a) / (double) num_threads;
    const double dx  = (b - a) * eps;

    for (int i = 0; i < num_threads; ++i) {
        int_args[i].a = a;
        a += len;
        int_args[i].b = a;

        int_args[i].dx = dx;
        int_args[i].func = func;
        int_args[i].id = i;

        //assert (pthread_create (&pid_arr[i], &pid_attr[i], _pthread_int_calc, &int_args[i]) == 0);
        assert (pthread_create (&pid_arr[i], NULL, _pthread_int_calc, &int_args[i]) == 0);
    }

    double res = 0;
    for (int i = 0; i < num_threads; ++i) {
        //assert (pthread_attr_destroy (&pid_attr[i]) == 0);
        assert (pthread_join (pid_arr[i], NULL) == 0);

        res += int_args[i].result;
    }

    return res * dx;
}

// todo: возможно что-то нужно сделать со стеком