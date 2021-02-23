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
    size_t num_bits = 4;
    BinArray* arr = baCreate (num_bits);
    if (arr == NULL) {
        perror ("baCreate");
        return -1;
    }

    size_t begin = 3;
    ssize_t len =  -1;

    baFillOne (arr, begin, len);

    BinArray* sub_arr = baGetSubArray (arr, begin, len);
    if (sub_arr == NULL) {
        perror ("baGetSubArray");
        baDestroy (&arr);
        return -1;
    }

    baDumpBufFull (arr);
    printf ("\n");
    baDumpBufFull (sub_arr);

    baDestroy (&sub_arr);
    baDestroy (&arr);

    /*
    size_t num_bits = 17;
    BinArray* arr = baCreate (num_bits), *index = baCreate (num_bits);
    if (arr == NULL || index == NULL) {
        perror ("baCreate");
        return -1;
    }

    baFillOneFull (arr);
    //baFillZeroFull (arr);
    baFillZeroFull (index);

    ssize_t size_dump = -1;
    size_t begin = 16;
    ssize_t len = -1;
    
    baSetOne (index, begin);
    baSetOne (index, begin + len - 1);
    baDumpBuf (index, 0, size_dump);

    baFillZero (arr, begin, len);
    //baFillOne (arr, begin, len);
    baDumpBuf (arr, 0, size_dump);

    for (int i = 0; i < 23; ++i) {
        //printf ("Find 1 (%2d): %2zi\n", i, baFindOne (arr, i, -1));
        printf ("Find 0 (%2d): %2zi\n", i, baFindZero (arr, i, -1));
    }

    baDestroy (&arr);
    baDestroy (&index);
    */
}