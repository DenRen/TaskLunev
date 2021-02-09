#include "BinArray.h"
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <assert.h>

//                         .
// size - in bits, because : 8
static inline int Bits2Bytes (int num_bits) {
    return num_bits / 8 + (num_bits % 8 != 0) * 8; 
}

int baRetain (struct BinArray *arr, int num_bits) {

    if (arr == NULL || num_bits <= 0) {
        errno = EINVAL;
        return -1;
    }

    int num_bytes  = Bits2Bytes (num_bytes);

    if ((arr->buf_ = (char*) calloc (sizeof (char),  num_bytes)) == NULL) {
        return -1;
    }
    
    arr->num_bits_ = num_bytes * 8;

    return 0;
}

int baResize   (struct BinArray *arr, int new_num_bits) {

    if (arr == NULL || new_num_bits <= 0 || arr->buf_ == NULL) {
        errno = EINVAL;
        return -1;
    }

    int new_num_bytes = Bits2Bytes (new_num_bits);
    char* new_buf = (char*) realloc (arr->buf_, sizeof (char) * new_num_bytes);
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
}