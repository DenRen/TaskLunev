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

    Logs,
    Test,

    Optimize, baFindOne
*/

typedef struct bin_array BinArray;

#ifdef __cplusplus
extern "C" {
#endif

// ===================\\
// Secondary functions --------------------------------------------------------
// ===================//

size_t baBits2Bytes (size_t num_bits);

void print_byte (uint8_t byte);
void print_byte_nl (uint8_t byte);
void bprint (uint8_t* bytes, size_t num_bytes);

// ==================\\
// Creation functions ---------------------------------------------------------
// ==================//

BinArray* baCreate (size_t num_bits);
int baResize (BinArray* arr, size_t new_num_bits);
int baDestroy (BinArray** arr);
BinArray* baGetClone (BinArray* arr);
BinArray *baGetSubArray (BinArray* arr, size_t begin, ssize_t len);
size_t baGetNumBits (BinArray* arr);

// =============================\\
// Getters and setters functions ----------------------------------------------
// =============================//

int baGetValue (BinArray *arr, size_t num_bit);
int baSetValue (BinArray* arr, size_t num_bit, bool val);
int baSetOne   (BinArray* arr, size_t num_bit);
int baSetZero  (BinArray* arr, size_t num_bit);

// ==============\\
// Find functions -------------------------------------------------------------
// ==============//

ssize_t baFind (BinArray* arr, size_t begin, ssize_t len, bool val);
ssize_t baFindOne  (BinArray* arr, size_t begin, ssize_t len);
ssize_t baFindZero (BinArray* arr, size_t begin, ssize_t len);

// ==============\\
// Fill functions -------------------------------------------------------------
// ==============//

int baFillOne  (BinArray* arr, size_t begin, ssize_t len);
int baFillZero (BinArray* arr, size_t begin, ssize_t len);

int baFillZeroFull (BinArray* arr);
int baFillOneFull  (BinArray* arr);

// ==============\\
// Dump functions -------------------------------------------------------------
// ==============//

int baDumpBuf (BinArray* arr, size_t begin, ssize_t len);
int baDumpBufFull (BinArray* arr);

// ================\\
// Foreach function -----------------------------------------------------------
// ================//

int baForeach (BinArray* arr,
             int (*cb) (BinArray* arr, bool element, void* data),
             void* data);

// ================\\
// Invert functions -----------------------------------------------------------
// ================//

int baInvert (BinArray* arr);
BinArray* baGetInvert (BinArray* arr);

#ifdef __cplusplus
}
#endif

#endif /* BINARRAY_H */