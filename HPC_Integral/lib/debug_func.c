#include "debug_func.h"
#include <stdio.h>
// ==============\\
// Print function -------------------------------------------------------------
// ==============//

void print_error_line (const char strerr[], const char name_file[], int line) {
    char str_error[256];
    sprintf (str_error, "%s\n"
                        "LINE: %s: %d\n", strerr, name_file, line);
    perror (str_error);
}