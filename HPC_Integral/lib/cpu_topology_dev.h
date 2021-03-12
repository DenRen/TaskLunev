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

#endif // CPU_TOPOLOGY_DEV_H