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

int main () {

    cpu_topology_t* cputop = cputopCreate ();

    cputopInit(cputop);
    
    cputopDestroy (cputop);

    //GetTopologyCPU ();
    //printf ("%s", __PRETTY_FUNCTION__);
    //printf ("%g\n", Integral (0, 3.141592, sin, 8));
}