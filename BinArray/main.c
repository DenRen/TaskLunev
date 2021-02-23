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

    ssize_t num_bits = 700 * 100;
    BinArray* arr = baCreate (num_bits);
    if (arr == NULL) {
        perror ("baCreate");
        return -1;
    }

    for (ssize_t pos = 789; pos < num_bits; ++pos) {
        baSetOne (arr, pos);

        ssize_t pos_find = baFindOne (arr, 265, -1);
        if (pos != pos_find) {
            printf ("pos: %zi, find: %zi\n", pos, pos_find);

            //baDestroy (&arr);
            //return 0;
        }

        baSetZero (arr, pos);
    }

    printf ("Good!\n");
    baDestroy (&arr);
}