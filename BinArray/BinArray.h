#ifndef BINARRAY_H
#define BINARRAY_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <sys/types.h>

/*
Вопросы:
    1) Нужно ли в функциях, которые не меняют объект либо указатель на него
    писать f (const BinArray *arr) или f (BinArray * const arr)?
*/

/*
Tasks:

    Дописать поиск False

    Logs,
    Test,

    Optimize _baSetValue, baFind_One
*/

typedef struct bin_array BinArray;

#ifdef __cplusplus
extern "C" {
#endif
    
BinArray *baCreate  (size_t num_bits);
int baResize  (BinArray* arr, size_t new_num_bits);
int baDestroy (BinArray** arr);

BinArray *baGetClone (BinArray* arr);
BinArray *baGetSubArray (BinArray* arr, size_t begin, ssize_t len);

size_t baGetNumBits (BinArray* arr);

int baGetValue (BinArray *arr, size_t num_bit);
int baSetValue (BinArray* arr, size_t num_bit, bool val);
int baSetOne   (BinArray* arr, size_t num_bit);
int baSetZero  (BinArray* arr, size_t num_bit);

int64_t baFind     (BinArray* arr, bool val);
int64_t baFindOne  (BinArray* arr);
int64_t baFindZero (BinArray* arr);

/*
int64_t baFind     (BinArray* arr, size_t begin, ssize_t len, bool val);
int64_t baFindOne  (BinArray* arr, size_t begin, ssize_t len);
int64_t baFindZero (BinArray* arr, size_t begin, ssize_t len);

int baFillOne  (BinArray* arr, size_t begin, ssize_t len);
int baFillZero (BinArray* arr, size_t begin, ssize_t len);

int baDumpBuf  (BinArray* arr, size_t begin, ssize_t len);
*/

int baFillOne  (BinArray* arr, size_t begin, ssize_t len);
int baFillZero (BinArray* arr);

int baDumpBuf  (BinArray* arr, ssize_t num_bytes);

int foreach (BinArray* arr,
             int (*cb) (BinArray* arr, bool element, void* data),
             void* data);

int baInvert (BinArray* arr); //ffs
BinArray* baGetInvert (BinArray* arr);

void print_byte (uint8_t byte);

#ifdef __cplusplus
}
#endif

#endif /* BINARRAY_H */