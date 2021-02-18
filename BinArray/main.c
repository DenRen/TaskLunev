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

    const size_t size = 1023;
    BinArray* bin_arr = baCreate (size);

    if (bin_arr == NULL) {
        perror ("baCreate");
        return -1;
    }

    baFillZero (bin_arr);

    const int num = 10;
    /*
    for (int i = 0; i < num; i++)
        printf ("%8d ", bin_arr.buf_[i]);
    printf ("\n");
    */

    baDumpBuf (bin_arr, num);

    //baSetValue (bin_arr, 3, 1);
    //baSetValue (bin_arr, 19, 1);
    
    //FillRandArrayBA (&bin_arr, 1);

    baDumpBuf (bin_arr, num);

    BinArray* inv_arr = baGetInvert (bin_arr);
    baInvert (bin_arr);
    
    baDumpBuf (inv_arr, num);
    baDumpBuf (bin_arr, num);

    printf ("Find 1: %zi\n", baFindOne  (bin_arr));
    printf ("Find 0: %zi\n", baFindZero (bin_arr));

    if (baDestroy (&bin_arr) == -1) {
        perror ("baRelease");
        return -1;
    }
    
    if (baDestroy (&inv_arr) == -1) {
        perror ("baRelease");
        return -1;
    }
}