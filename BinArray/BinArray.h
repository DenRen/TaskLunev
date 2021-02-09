#ifndef BINARRAY_H
#define BINARRAY_H

#include <stdbool.h>
#include <stdint.h>

/*
Tasks:
    Full methods:
        Нормальный поиск
        Вставка,

    Logs,
    Test,

    Optimize _baSetValue
*/

struct BinArray {
    uint8_t* buf_;
    int num_bits_;
};

int baRetain  (struct BinArray* arr, int num_bits);
int baResize  (struct BinArray* arr, int new_num_bits);
int baRelease (struct BinArray* arr);

int baGetValue (const struct BinArray arr, int num_bit);
int baSetValue (struct BinArray arr, int num_bit, bool val);

int baFind (struct BinArray arr, bool val);
int baFind_True (struct BinArray arr);

void baDumpBuf (const struct BinArray arr, int num_bytes);

#endif /* BINARRAY_H */