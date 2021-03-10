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
/*
todo: Ускоряет не линейно. С этим нужно будет разобраться
*/
int main (int argc, char* argv[]) {
    if (argc != 2) {
        printf ("Please enter the number thread. F.e.: ./main.out 4\n");
        return -1;
    }
    
    const int num_threads = atoi (argv[1]);
    if (num_threads <= 0) {
        printf ("Please, enter the positive number thread\n");
        return -1;
    }

    printf ("%g\n", Integral (0, 3.141592, sin, num_threads));
}