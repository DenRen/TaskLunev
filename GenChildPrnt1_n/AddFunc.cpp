#include <stdio.h>
#include <cstdarg>
#include <unistd.h>
#include "AddFunc.h"

void NOP () {}

void close (int count, int first, ...) {
    close (first);

    va_list ap;
    va_start (ap, first);
    for (int i = 1; i < count; i++)
        close (va_arg (ap, int));
}