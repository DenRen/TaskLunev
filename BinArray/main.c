#include <stdio.h>
#include "BinArray.h"
//#include <random>

typedef int el_t;

int find_max (BinArray* c, bool el, void* data) {
    bool max = *(bool*) data;
    if (el > max)
        *(bool*) data = el;

    return 0;
}

// todo dump
#include <strings.h>
int main () {
    printf ("\n");

    ssize_t num_bits = 100;
    BinArray* arr = baCreate (num_bits);
    if (arr == NULL) {
        perror ("baCreate");
        return -1;
    } 

    baDumpBufFull (arr);

    baFillOne (arr, 4, 93);

    baDumpBufFull (arr);
    
    baDestroy (&arr);
}