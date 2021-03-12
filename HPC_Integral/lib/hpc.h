#ifndef HPC_H
#define HPC_H

#define _GNU_SOURCE
#include <sched.h>

double hpcIntegral (double a, double b, double (* func) (double), const unsigned num_threads);
double hpcSimpleIntegral (double a, double b, double (*func) (double));

int hpcSetPolicy (const int policy, unsigned priority);

#endif // HPC_H