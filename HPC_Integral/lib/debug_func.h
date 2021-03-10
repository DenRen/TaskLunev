#ifndef DEBUG_FUNC_H
#define DEBUG_FUNC_H

void print_error_line (const char strerr[], const char name_file[], int line);

#define PRINT_ERROR(str) print_error_line (str, __FILE__, __LINE__);

#define CHECK_PTR(ptr)                                              \
    if (ptr == NULL) {                                              \
        print_error_line ("Pointer is NULL", __FILE__, __LINE__);   \
        return -1;                                                  \
    }

#endif // DEBUG_FUNC_H