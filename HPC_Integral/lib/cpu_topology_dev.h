#ifndef CPU_TOPOLOGY_DEV_H
#define CPU_TOPOLOGY_DEV_H

typedef struct _cpu_topology_t cpu_topology_t;
typedef struct _logic_cpu_t logic_cpu_t;

// ===========\\
// Main struct -------------------------------------------------------------
// ===========//

typedef struct _logic_cpu_t {
    int id;
    int core_id;
} logic_cpu_t;

typedef struct _cpu_topology_t {
    int num_logic_cpu;
    logic_cpu_t* logic_cpus;
} cpu_topology_t;


char* _readCpuTopologyFile (const char name_file[]);

int _cputopFillLogicCPU (cpu_topology_t* cputop, char* str);
int _cputopFillCoreCPU (cpu_topology_t* cputop);

int _cputopInitLogicCPU (cpu_topology_t* cputop);
int _cputopInitCoreCPU (cpu_topology_t* cputop);

void _cputopSortCoreId (cpu_topology_t* cputop);
void _cputopSwapLogicCpu (logic_cpu_t* first, logic_cpu_t* second);

void _cputopDestroy (cpu_topology_t* cputop);

#endif // CPU_TOPOLOGY_DEV_H