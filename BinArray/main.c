#include <stdio.h>
#include "BinArray.h"
#include <random>

typedef int el_t;

int find_max (BinArray* c, bool el, void* data) {
    bool max = *(bool*) data;
    if (el > max)
        *(bool*) data = el;

    return 0;
}

int main () {

    printf ("\n");
    const size_t size = 15;
    BinArray* bin_arr = baCreate (size);

    if (bin_arr == NULL) {
        perror ("baCreate");
        return -1;
    }

    std::random_device rd;
    std::mt19937 gen (rd ());

    using uni_dest = std::uniform_int_distribution <>;
    uni_dest bool_rand (0, 1);
    
    const size_t num_bits = baGetNumBits (bin_arr);
    for (size_t i = 0; i < num_bits; ++i)
        baSetValue (bin_arr, i, bool_rand (gen));

    // baFillZero (bin_arr);

    const int num = 7;
    /*
    for (int i = 0; i < num; i++)
        printf ("%8d ", bin_arr.buf_[i]);
    printf ("\n");
    */

    baDumpBuf (bin_arr, num);

    //baSetValue (bin_arr, 3, 1);
    //baSetValue (bin_arr, 19, 1);
    
    //FillRandArrayBA (&bin_arr, 1);

    /*const size_t shift = 1;
    BinArray* sub_arr = baGetSubArray (bin_arr, shift, 5);
    
    for (int i = 0; i < shift; ++i) {
        printf (" ");
        if (i % 8 == 0 && i != 0)
            printf (" ");
    }
    baDumpBuf (sub_arr, num);
    
    baDestroy (&sub_arr);
    */

    size_t begin = 3;
    ssize_t len = 10;
    
    BinArray* ind_arr = baCreate (size);
    baSetOne (ind_arr, begin);
    baSetOne (ind_arr, begin + len - 1);
    baDumpBuf (ind_arr, num);

    baFillOne (bin_arr, begin, len);
    baDumpBuf (bin_arr, num);

    printf ("\n");
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