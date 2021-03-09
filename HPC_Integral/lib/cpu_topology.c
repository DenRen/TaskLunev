#include "cpu_topology.h"
#include "cpu_topology_dev.h"

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define PATH_CPU     "/sys/devices/system/cpu/"
#define FILE_ONLINE  "online"
#define DIR_TOPOLOGY "cpu%d/topology"
#define FILE_COREID  "core_id"

const int MAX_LEN_PATH = 128;
const int MAX_SIZE_TOPOLGY_FILE = 128;  // F.e cpu/online, cpu#/topology/core_id

/*
for ((count = 0; count < 8; count++))
do
cat cpu$count/topology/* | tr -s '\n' ' '
echo ""
done
*/

/*
core_id: the CPU core ID of cpu#. Typically it is the
		hardware platform's identifier (rather than the kernel's).
		The actual value is architecture and platform dependent.

thread_siblings: internel kernel map of cpu#'s hardware
		threads within the same core as cpu#
*/

// ==============\\
// Error function -------------------------------------------------------------
// ==============//

void print_error_line (const char strerr[], const char name_file[], int line) {
    char str_error[256];
    sprintf (str_error, "%s\n"
                        "LINE: %s: %d\n", strerr, name_file, line);
    perror (str_error);
}

#define CHECK_PTR(ptr)                                              \
    if (ptr == NULL) {                                              \
        print_error_line ("Pointer is NULL", __FILE__, __LINE__);   \
        return -1;                                                  \
    }

// ===========\\
// Main struct -------------------------------------------------------------
// ===========//

typedef struct {
    int id;
    int core_id;
} logic_cpu_t;

typedef struct _cpu_topology_t {
    int num_logic_cpu;
    logic_cpu_t* logic_cpus;
} cpu_topology_t;

// ==============\\
// Init functions -------------------------------------------------------------
// ==============//

cpu_topology_t* cputopCreate () {
    return (cpu_topology_t*) calloc (1, sizeof (cpu_topology_t));
}

int cputopDestroy (cpu_topology_t* cputop) {
    CHECK_PTR (cputop);

    free (cputop->logic_cpus);
    cputop-> num_logic_cpu = -1;
}

int cputopInit (cpu_topology_t* cputop) {
    CHECK_PTR (cputop);

    cputopInitLogicCPU (cputop);    // Read cpu/online
    cputopInitCPU (cputop);         // Read cpu#/topology/core_id
}

// ====================\\
// Init functions [DEV] -------------------------------------------------------
// ====================//

const int LOGIC_CPU_MAX = 256;

char* _readCpuTopologyFile (const char name_file[]) {
    int fd = open (name_file, O_RDONLY);
    if (fd == -1) {
        perror ("open");
        return NULL;
    }

    struct stat stat_fd = {0};
    if (fstat (fd, &stat_fd) == -1) {
        perror ("fstat");
        close (fd);
        return NULL;
    }

    const int str_len = stat_fd.st_size;
    char* buf = (char*) calloc (str_len + 1, 1);

    const int len = read (fd, buf, str_len);
    if (len == -1 || len == 0) {
        perror ("read");
        close (fd);
        return NULL;
    }

    close (fd);

    buf[len] = '\0';

    return buf;
}

int _cputopFillLogicCPU (cpu_topology_t* cputop, char* str) {
    CHECK_PTR (cputop);
    CHECK_PTR (str);

    char* str2 = NULL, *token = NULL;

    cputop->logic_cpus = (logic_cpu_t*) calloc (LOGIC_CPU_MAX, sizeof (logic_cpu_t));
    
    int num_cpus = 0;

    for (char* str1 = str; ; str1 = NULL) {
        if ((token = strtok (str1, ",")) == NULL)
            break;
        
        char* pos_delim = strchr (token, '-');
        if (pos_delim == NULL) {    // 1
            cputop->logic_cpus[num_cpus++].id = atoi (token);
        }
        else {                      // 1-3
            int cur_log_cpu = atoi (token) + 1, max_log_cpu = atoi (pos_delim + 1);
            do {
                cputop->logic_cpus[num_cpus++].id = cur_log_cpu++;
            } while (cur_log_cpu <= max_log_cpu);
        }
    }

    cputop->num_logic_cpu = num_cpus;
    cputop->logic_cpus = (logic_cpu_t*) realloc (cputop->logic_cpus, num_cpus * sizeof (logic_cpu_t));
    
    return 0;
}
int _cputopFillCoreCPU (cpu_topology_t* cputop) {
    CHECK_PTR (cputop);
    CHECK_PTR (cputop->logic_cpus);
    
    if (cputop->num_logic_cpu <= 0) {
        errno = EINVAL;
        print_error_line ("(cputop->num_logic_cpu <= 0) is false", __FILE__, __LINE__);
        return -1;
    }

    const int num_logic_cpus = cputop->num_logic_cpu;
    for (int i = 0; i < num_logic_cpus; ++i) {
        const int num_logic_cpu = cputop->logic_cpus[i].id;
        
        char path_file[MAX_LEN_PATH] = {0};
        sprintf (path_file, PATH_CPU DIR_TOPOLOGY "/" FILE_COREID, num_logic_cpu);

        char* str_core_id = _readCpuTopologyFile (path_file);
        CHECK_PTR (str_core_id);

        cputop->logic_cpus[i].core_id = atoi (str_core_id);

        free (str_core_id);
    }

    return 0;
}

int cputopInitLogicCPU (cpu_topology_t* cputop) {
    CHECK_PTR (cputop);

    char* str = _readCpuTopologyFile (PATH_CPU FILE_ONLINE);
    if (str == NULL) {
        perror ("_readFile");
        return -1;
    }

    if (_cputopFillLogicCPU (cputop, str) == -1) {
        perror ("_cputopFillLogicCPU");
        free (str);
        return -1;
    }

    free (str);

}
int cputopInitCPU (cpu_topology_t* cputop) {
    CHECK_PTR (cputop);

    if (_cputopFillCoreCPU (cputop) == -1) {
        print_error_line ("_cputopFillCoreCPU", __FILE__, __LINE__);
        return -1;
    }
}