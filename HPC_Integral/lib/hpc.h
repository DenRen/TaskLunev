#ifndef HPC_H
#define HPC_H

#define _GNU_SOURCE
#include <sched.h>

#define LINEAR_TIME_CALC
//#define VECT_CALC

typedef struct {
    double a, b;
    double (*func) (double x);
    unsigned num_threads;
} hpc_int_arg_t;

double hpcIntegral (hpc_int_arg_t* hpc_int_arg);
double hpcSimpleIntegral (double a, double b, double (*func) (double));

int hpcSetPolicy (const int policy, unsigned priority);

#endif // HPC_H
