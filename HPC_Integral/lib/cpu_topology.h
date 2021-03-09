#ifndef CPU_TOPOLOGY_H
#define CPU_TOPOLOGY_H

typedef struct _cpu_topology_t cpu_topology_t;


cpu_topology_t* cputopCreate ();
int cputopDestroy (cpu_topology_t* cputop);
int cputopInit (cpu_topology_t* cputop);

void GetTopologyCPU ();
int GetNumLogicCPU ();

#endif // CPU_TOPOLOGY_H