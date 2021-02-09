#ifndef BINARRAY_H
#define BINARRAY_H

/*

Tasks:
    Full methods,
    Logs,
    Test
*/

struct BinArray {
    char* buf_;
    int num_bits_;
};

int baRetain   (struct BinArray *arr, int num_bits);
int baResize   (struct BinArray *arr, int new_num_bits);
int baRelease (struct BinArray *arr);

#endif /* BINARRAY_H */