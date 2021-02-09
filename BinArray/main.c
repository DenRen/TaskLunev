#include <stdio.h>
#include "BinArray.h"

int main () {
    struct BinArray bin_arr;
    const int size = 100;
    if (baRetain (&bin_arr, size) == -1) {
        perror ("baRetain");
        return -1;
    }

    if (baResize (&bin_arr, 1100000000) == -1) {
        perror ("baResize");
        baRelease (&bin_arr);
        return -1;
    }

    if (baRelease (&bin_arr) == -1) {
        perror ("baRelease");
        return -1;
    }
}