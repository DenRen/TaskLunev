#include "BinArray.h"
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

static void _mynop() {}

#define CHECK_PBA(ptr_arr)                                                   \
    if (ptr_arr == NULL || ptr_arr->buf_ == NULL || ptr_arr->num_bits_ == 0) \
    {                                                                        \
        errno = EINVAL;                                                      \
        return -1;                                                           \
    }                                                                        \
    _mynop()

#define CHECK_BA(arr)                           \
    if (arr.buf_ == NULL || arr.num_bits_ == 0) \
    {                                           \
        errno = EINVAL;                         \
        return -1;                              \
    }                                           \
    _mynop()

//                         .
// size - in bits, because : 8
static inline int Bits2Bytes (int num_bits) {
    return num_bits / 8 + (num_bits % 8 != 0);
}

int baRetain (struct BinArray *arr, size_t num_bits) {

    if (arr == NULL || num_bits <= 0) {
        errno = EINVAL;
        return -1;
    }

    int num_bytes = Bits2Bytes (num_bits);

    if ((arr->buf_ = (uint8_t *) calloc (sizeof (uint8_t), num_bytes)) == NULL) {
        return -1;
    }

    arr->num_bits_ = num_bits;

    return 0;
}

int baFill_One (struct BinArray arr) {
    CHECK_BA (arr);

    memset (arr.buf_, 0xFF, Bits2Bytes (arr.num_bits_));

    return 0;
}

int baFill_Zero (struct BinArray arr) {
    CHECK_BA (arr);

    memset (arr.buf_, 0x00, arr.num_bits_ / 8);

    return 0;
}

int baResize (struct BinArray *arr, size_t new_num_bits) {

    CHECK_PBA (arr);

    int new_num_bytes = Bits2Bytes (new_num_bits);
    uint8_t *new_buf = (uint8_t *) realloc (arr->buf_, sizeof (uint8_t) * new_num_bytes);
    if (new_buf == NULL) {
        return -1;
    }

    arr->buf_ = new_buf;
    arr->num_bits_ = new_num_bytes;

    return 0;
}

int baRelease (struct BinArray *arr) {

    if (arr == NULL) {
        errno = EINVAL;
        return -1;
    }

    free (arr->buf_);

    arr->buf_ = NULL;
    arr->num_bits_ = 0;

    return 0;
}

// Without checking for correctness (for speed)
static inline bool _baGetValue (struct BinArray arr, size_t num_bit) {
    uint8_t obj = *(arr.buf_ + num_bit / 8);

    return (obj & (1U << (7 - num_bit % 8)));
}

int baGetValue (struct BinArray arr, size_t num_bit) {

    CHECK_BA (arr);

    if (num_bit > arr.num_bits_) {
        errno = E2BIG;
        return -1;
    }

    return _baGetValue (arr, num_bit);
}

// Without checking for correctness (for speed)
static inline void _baSetValue (struct BinArray arr, size_t num_bit, bool val) {
    uint8_t *obj = arr.buf_ + num_bit / 8;
    uint8_t mask = 1U << (7 - num_bit % 8);

    if (val == 1)
        *obj |= mask;
    else
        *obj &= ~mask;
}

int baSetValue (struct BinArray arr, size_t num_bit, bool val) {

    CHECK_BA(arr);

    if (num_bit > arr.num_bits_) {
        errno = E2BIG;
        return -1;
    }

    _baSetValue(arr, num_bit, val);

    return 0;
}

int baFind(struct BinArray arr, bool val) {
    // 64 bits CPU

    if (val)
        return baFind_True(arr);
}

static inline int _baFindBiteInByte (uint8_t byte) {
    uint8_t mask = 1U << 7;

    for (int i = 0; i < 8; ++i)
        if (byte & (mask >> i))
            return i;
}

int baFind_True (struct BinArray arr) {
    const uint64_t mask_u64 = 0xFFFFFFFFFFFFFFFF; // Search by 8 bytes
    const uint64_t *buf_u64 = (uint64_t *)arr.buf_;

    int num_u64 = arr.num_bits_ / (8 * 8);
    int i = 0, path = 0;
    for (; i < num_u64; ++i)
        if (buf_u64[i] & mask_u64)
            break;

    path = i * 64;

    if (i == num_u64) {
        const int num_u8 = (arr.num_bits_ - 64 * num_u64) / 8 + 1;
        const uint8_t *buf_u8 = (const uint8_t *)(buf_u64 + i);

        for (i = 0; i < num_u8; ++i)
            if (buf_u8[i] & 0xFF) { // Equal  if (buf_u8[i])
                path += i * 8;

                int path_bit = _baFindBiteInByte(buf_u8[i]);

                if (path + path_bit >= arr.num_bits_)
                    return -1;
                else
                    return path + path_bit;
            }

        return -1;
    }
    else
    {
        const uint8_t *buf_u8 = (uint8_t *)(buf_u64 + i);

        for (i = 0; i < 8; i++)
            if (buf_u8[i] & 0xFF)
            {
                path += i * 8;

                return path + _baFindBiteInByte(buf_u8[i]);
            }
    }
}

void baDumpBuf (const struct BinArray arr, size_t num_bytes) {
    assert (arr.buf_ != NULL);

    int num_bits = 0;
    if (num_bytes < 0 || arr.num_bits_ < 8 * num_bytes) {
        num_bits = arr.num_bits_;
    }

    for (int i = 0; i < num_bits; ++i) {
        printf ("%d", _baGetValue (arr, i));

        if ((i + 1) % 8 == 0)
            printf (" ");
    }

    printf("\n");
}