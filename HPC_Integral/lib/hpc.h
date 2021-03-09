#ifndef HPC_H
#define HPC_H

double SimpleIntegral (double a, double b, double (*func) (double));
double Integral (double a, double b, double (* func) (double), int num_threads);

#endif // HPC_H