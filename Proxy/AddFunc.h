#ifndef TASK2FD_ADDFUNC_H
#define TASK2FD_ADDFUNC_H

#include <cstdlib>
#include <cerrno>

void close (int count, int first, ...);

long GetNumber (const char *str);

#define ZERO_ARRAY(array) memset (array, 0, sizeof (array))
#define SET_IN_ZERO(mem)  memset (&mem, 0, sizeof (mem))

#endif //TASK2FD_ADDFUNC_H