#include <stdio.h>
#include <cstdarg>
#include <unistd.h>

#include "AddFunc.h"

void close (int count, int first, ...) {
    close (first);

    va_list ap;
    va_start (ap, first);
    for (int i = 1; i < count; i++)
        close (va_arg (ap, int));
}

long GetNumber (const char *str) {
    errno = 0;
    long number = strtol (str, nullptr, 10);



    return number;
}
