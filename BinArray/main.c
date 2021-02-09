#include <stdio.h>
#include "BinArray.h"

int main () {
    struct BinArray bin_arr;
    const int size = 15;
    if (baRetain (&bin_arr, size) == -1) {
        perror ("baRetain");
        return -1;
    }

    //printf ("Number: %d\n", (bin_arr.buf_[0] = 0b00000100));

    const int num = 2;
    for (int i = 0; i < num; i++)
        printf ("%8d ", bin_arr.buf_[i]);
    printf ("\n");

    baDumpBuf (bin_arr, num);

    baSetValue (bin_arr, 13, 1);

    baDumpBuf (bin_arr, num);

    printf ("Find 1: %d\n", baFind_True (bin_arr));

    if (baRelease (&bin_arr) == -1) {
        perror ("baRelease");
        return -1;
    }
}