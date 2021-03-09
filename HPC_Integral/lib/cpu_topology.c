#include "cpu_topology.h"
#include "cpu_topology_dev.h"

#define PATH_CPU     "/sys/devices/system/cpu/"
#define FILE_ONLINE  "online"
#define DIR_TOPOLOGY "cpu%d/topology"
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "../../BinArray/BinArray.h"

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

void print_error_line (const char name_file[], int line) {
    char str_error[256];
    sprintf (str_error, "Pointer is NULL\n"
                        "LINE: " __FILE__ ": %d\n", __LINE__);
    perror (str_error);
}

#define CHECK_PTR(ptr)                          \
    if (ptr == NULL) {                          \
        print_error_line (__FILE__, __LINE__);  \
        return -1;                              \
    }

// ==================\\
// CPU topology struct -------------------------------------------------------------
// ==================//

typedef struct {
    int id;
    int core_id;
} logic_cpu_t;

typedef struct {
    int num_logic_cpu;
    logic_cpu_t* logic_cpus;
} cpu_topology_t;

// ======================\\
// CPU topology functions -------------------------------------------------------------
// ======================//

cpu_topology_t* cputopCreate () {
    return (cpu_topology_t*) calloc (1, sizeof (cpu_topology_t));
}

int cputopDestroy (cpu_topology_t* cputop) {
    CHECK_PTR (cputop);

    free (cputop->logic_cpus);
    cputop->logic_cpus = -1;
}

int cputopInit (cpu_topology_t* cputop) {
    CHECK_PTR (cputop);

    // todo: fill main 
}

void GetTopologyCPU () {
    const int num_cpus = GetNumLogicCPU ();
    printf ("num_cpus: %d\n", num_cpus);

    char dir_topology[sizeof (PATH_CPU) + sizeof (DIR_TOPOLOGY) + 16];

    unsigned thread_cpu[num_cpus];

    for (int num_cpu = 0; num_cpu < num_cpus; ++num_cpu) {
        
        sprintf (dir_topology, PATH_CPU DIR_TOPOLOGY "/core", num_cpu);

        //int fd = open ();
    }
}

char* _readFile (const char name_file[]) {
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

// str != NULL
int _parseString2SumNumber (char* str) {
    char* str2 = NULL, *token = NULL;

    int num = 0;

    for (char* str1 = str; ; str1 = NULL) {
        if ((token = strtok (str1, ",")) == NULL)
            break;
        
        char* pos_delim = strchr (token, '-');
        if (pos_delim == NULL)
            ++num;
        else
            num += atoi (pos_delim + 1) - atoi (token) + 1;
    }

    return num;
}

int GetNumLogicCPU () {
    
    char* str = _readFile (PATH_CPU FILE_ONLINE);
    if (str == NULL) {
        perror ("_readFile");
        return -1;
    }

    int numLogicCPU = _parseString2SumNumber (str);

    free (str);

    return numLogicCPU;
}