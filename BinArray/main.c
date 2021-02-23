#include <stdio.h>
#include "BinArray.h"

typedef int el_t;

int find_max (BinArray* c, bool el, void* data) {
    bool max = *(bool*) data;
    if (el > max)
        *(bool*) data = el;

    return 0;
}

int main () {
    printf ("\n");

    ssize_t num_bits = 100;
    BinArray* arr = baCreate (num_bits);
    if (arr == NULL) {
        perror ("baCreate");
        return -1;
    }
      
    baFillOneFull (arr);

    baSetZero (arr, 70);

    baDumpBufFull (arr);
    printf ("Find 0: %zi\n", baFindZero (arr, 3, -1));
    
    //

    baFillZeroFull (arr);

    baSetOne (arr, 70);

    baDumpBufFull (arr);
    printf ("Find 1: %zi\n", baFindOne (arr, 3, -1));

    baDestroy (&arr);
}