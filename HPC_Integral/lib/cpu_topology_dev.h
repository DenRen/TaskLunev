#ifndef CPU_TOPOLOGY_DEV_H
#define CPU_TOPOLOGY_DEV_H

typedef struct _logic_cpu_t logic_cpu_t;

char* _readCpuTopologyFile (const char name_file[]);

int _cputopFillLogicCPU (cpu_topology_t* cputop, char* str);
int _cputopFillCoreCPU (cpu_topology_t* cputop);

int cputopInitLogicCPU (cpu_topology_t* cputop);
int cputopInitCPU (cpu_topology_t* cputop);

void _cputopSortCoreId (cpu_topology_t* cputop);
void _cputopSwapLogicCpu (logic_cpu_t* first, logic_cpu_t* second);

#endif // CPU_TOPOLOGY_DEV_H