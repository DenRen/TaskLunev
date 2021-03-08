#include "cpu_topology.h"

#define PATH_CPU "/sys/devices/system/cpu/"
#define DIR_ONLINE "online"

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int GetNumLogicCPU () {
    int fd = open (PATH_CPU DIR_ONLINE, O_RDONLY);
    if (fd == -1) {
        perror ("open");
        return -1;
    }

    const int size_buf = 64;
    char buf[size_buf + 1];

    const int len = read (fd, buf, size_buf);
    
    if (len == -1 || len == 0) {
        perror ("read");
        close (fd);
        return -1;
    }

    buf[len] = '\0';
    
    char* str1 = buf, *str2 = NULL, *token = NULL;

    int num = 0;

    for (char* str1 = buf; ; str1 = NULL) {
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

// TODO парсить 0-1, 3, и т.д.

