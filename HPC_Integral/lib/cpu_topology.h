#ifndef CPU_TOPOLOGY_H
#define CPU_TOPOLOGY_H

typedef struct _cpu_topology_t cpu_topology_t;

#include <stdbool.h>

cpu_topology_t* cputopCreate ();
int cputopDestroy (cpu_topology_t** cputop);
int cputopInit    (cpu_topology_t* cputop);

int cputopGetNumLogicCPU  (cpu_topology_t* cputop);
int cputopGetNumCoreCPU   (cpu_topology_t* cputop);

int cputopGetLogicCpuId (cpu_topology_t* cputop, int num_logic_cpu);

int cputopDump (cpu_topology_t* cputop);

int cputopSortUniqSetsCoreId (cpu_topology_t* cputop);

#endif // CPU_TOPOLOGY_H