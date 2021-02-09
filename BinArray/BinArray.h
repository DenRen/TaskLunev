#ifndef BINARRAY_H
#define BINARRAY_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/*
Tasks:
    Full methods:
        Вставка,

    Logs,
    Test,

    Optimize _baSetValue, baFind_True
*/

struct BinArray {
    uint8_t* buf_;
    size_t num_bits_;
};

#ifdef __cplusplus
extern "C" {
#endif
    
int baRetain  (struct BinArray* arr, size_t num_bits);
int baResize  (struct BinArray* arr, size_t new_num_bits);
int baRelease (struct BinArray* arr);

int baGetValue (const struct BinArray arr, size_t num_bit);
int baSetValue (struct BinArray arr, size_t num_bit, bool val);

int baFind (struct BinArray arr, bool val);
int baFind_True (struct BinArray arr);

int baFill_One  (struct BinArray arr);
int baFill_Zero (struct BinArray arr);

void baDumpBuf (const struct BinArray arr, size_t num_bytes);

#ifdef __cplusplus
}
#endif

#endif /* BINARRAY_H */