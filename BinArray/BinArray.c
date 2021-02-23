#include "BinArray.h"
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

/*
TODO
1) До конца не оптимизированы baGetSubArray, baGetInvert
    Им нужно добавить оптимизацию 64 битной кратности размера массива
*/


// ==========\\
// Main types -----------------------------------------------------------------
// ==========//

typedef uint8_t* buf_t;

typedef struct bin_array {
    buf_t buf_;
    size_t num_bits_;
} BinArray;

// ===================\\
// Secondary functions --------------------------------------------------------
// ===================//

#ifdef PROBABLE_FAULT_ALLOCATORS
void* ba_calloc (size_t nmemb, size_t size) {
    if (size == baBits2Bytes (KILL_ALLOCATOR))
        return NULL;
    else
    
#else
void* ba_calloc (size_t nmemb, size_t size) {
#endif

    return calloc (nmemb, size);
}

#ifdef PROBABLE_FAULT_ALLOCATORS
void* ba_reallocarray (void* ptr, size_t nmemb, size_t size) {
    if (size == baBits2Bytes (KILL_ALLOCATOR))
        return NULL;
    else
    
#else
void* ba_reallocarray (void* ptr, size_t nmemb, size_t size) {
#endif
    //#include <stdlib.h>
    //return reallocarray (ptr, nmemb, size);
    return realloc (ptr, nmemb * size);
}

size_t baGetSizeArrayIn8Byte (size_t num_bits) {
    return num_bits  / 64 + (num_bits % 64 != 0);
}
size_t baBits2Bytes (size_t num_bits) {
    return num_bits / 8 + (num_bits % 8 != 0);
}

size_t _baGetNumBits (BinArray* arr) {
    return arr->num_bits_;
}
size_t baGetNumBits (BinArray* arr) {
    return _baGetNumBits (arr);
}

void print_byte (uint8_t byte) {
    char str[9] = "";
    for (uint8_t i = 0; i < 8; ++i)
        str[i] = '0' + ((byte & ((1U << 7) >> i)) != 0);

    str[8] = '\0';
    printf ("%s", str);
}
void print_byte_nl (uint8_t byte) {
    print_byte (byte);
    printf ("\n");
}
void bprint (uint8_t* bytes, size_t num_bytes) {
    for (int i = 0; i < num_bytes; ++i) {
        print_byte (bytes[i]);  
        printf (" ");
    }
}

// ===============\\
// Check functions ------------------------------------------------------------
// ===============//

bool baCheckArr (BinArray arr) {
    return (arr.buf_ != NULL) && (arr.buf_ != 0);
}

bool baCheckPtr (BinArray* arr) {
    return arr != NULL && baCheckArr (*arr);
}

// Effctive check and calculate input args
bool baCheckCalcArg (BinArray* arr, size_t begin, ssize_t* len) {
    
    if (baCheckPtr (arr)) {

        const size_t num_bits = _baGetNumBits (arr);

        if (*len == -1) {
            if (begin < num_bits) {
                *len = num_bits - begin;
                return true;
            } else
                return false;
        } else if (*len >= 0)
            return (begin + *len) < num_bits + 1;
        else 
            return false;
    } else
        return false;
}

static void _mynop() {}

#define CHECK_PBA(ptr_arr)              \
    if (baCheckPtr (ptr_arr) == false)  \
    {                                   \
        errno = EINVAL;                 \
        return -1;                      \
    }                                   \
    _mynop()

// This check will be remove in release
#define CHECK_BA(arr)                   \
    if (baCheckArr (arr) == false)      \
    {                                   \
        errno = EINVAL;                 \
        return -1;                      \
    }                                   \
    _mynop()

// ==================\\
// Creation functions ---------------------------------------------------------
// ==================//

BinArray* baCreate (size_t num_bits) {
    
    if (num_bits == 0) {
        errno = EINVAL;
        return NULL;
    }

    BinArray* arr = (BinArray*) ba_calloc (1, sizeof (BinArray));
    if (arr == NULL)
        return NULL;

    size_t num_bytes = 8 * baGetSizeArrayIn8Byte (num_bits);
    if ((arr->buf_ = (uint8_t *) ba_calloc (sizeof (uint8_t), num_bytes)) == NULL) {
        free (arr);
        return NULL;
    }

    arr->num_bits_ = num_bits;

    return arr;
}
int baResize (BinArray* arr, size_t new_num_bits) {

    CHECK_PBA (arr);
    if (new_num_bits == 0)
        return -1;
    
    uint8_t* new_buf = (uint8_t*) ba_reallocarray (arr->buf_,
                                                   8 * baGetSizeArrayIn8Byte (new_num_bits),
                                                   sizeof (uint8_t));
    if (new_buf == NULL)
        return -1;

    arr->buf_ = new_buf;
    arr->num_bits_ = new_num_bits;

    return 0;
}
int baDestroy (BinArray** arr) {

    if (arr == NULL) {
        errno = EINVAL;
        return -1;
    }

    free ((*arr)->buf_);
    free (*arr);

    *arr = NULL;

    return 0;
}
BinArray* baGetClone (BinArray* arr) {
    if (baCheckPtr (arr) == false)
        return NULL;

    BinArray* clone = baCreate (arr->num_bits_);
    if (clone == NULL)
        return NULL;

    memcpy (clone->buf_, arr->buf_,  8 * baGetSizeArrayIn8Byte (clone->num_bits_));

    return clone;
}
BinArray *baGetSubArray (BinArray* arr, size_t begin, ssize_t len) {
    if (baCheckCalcArg (arr, begin, &len) == false) 
        return NULL;

    BinArray* clone = baCreate (len);
    if (clone == NULL)
        return NULL;

    uint8_t* sub_buf = clone->buf_;

    uint8_t* buf = arr->buf_ + begin / 8;
    const uint8_t rshift = begin % 8;
    
    if (rshift == 0)
        memcpy (sub_buf, buf, baBits2Bytes (len));
    else {
        const ssize_t over_bytes = baBits2Bytes (len + rshift);
        
        ssize_t num_u56 = (over_bytes - 1) / 7; // 7 * x + 1
        if (baBits2Bytes (len) < 8)
            num_u56 = 0;
        
        for (size_t i = 0; i < num_u56; ++i) {
            *((uint64_t*) sub_buf) = *((uint64_t*) buf) >> rshift;
            sub_buf += 7;
            buf += 7;
        }

        bool state = over_bytes != baBits2Bytes (len); // Для несовпадения размеров
        
        int num_u8 = over_bytes - 7 * num_u56 - 1;
        if (state)
            --num_u8;
        
        for (int i = 0; i < num_u8; ++i) {
            *(uint16_t*) sub_buf = (*(uint16_t*) buf) >> rshift;
            ++sub_buf;
            ++buf;
        }

        if (state)
            *sub_buf = (*(uint16_t*) buf) >> rshift;
        else
            *sub_buf = *buf >> rshift;
    }

    return clone;
}

// =============================\\
// Getters and setters functions ----------------------------------------------
// =============================//

static uint8_t* _baGetPtrByte (buf_t buf, size_t num_byte) {
    return buf + num_byte;
} 
static uint8_t _baGetByte (buf_t buf, size_t num_byte) {
    return *_baGetPtrByte (buf, num_byte);
}
static uint8_t _baGetMaskBitFromByte (uint8_t num_bit) {
    return (uint8_t) (1U << num_bit);
}
static bool _baGetBitFromByte (uint8_t byte, uint8_t num_bit) {
    return byte & _baGetMaskBitFromByte (num_bit);
}

static /* inline*/ bool _baGetValue (buf_t buf, size_t num_bit) {
    uint8_t byte = _baGetByte (buf, num_bit / 8);
    return _baGetBitFromByte (byte, num_bit % 8);
}

int baGetValue (BinArray *arr, size_t num_bit) {

    CHECK_PBA (arr);

    if (num_bit > arr->num_bits_) {
        errno = E2BIG;
        return -1;
    }

    return _baGetValue (arr->buf_, num_bit);
}

static /* inline*/ void _baSetOne (buf_t buf, size_t num_bit) {
    uint8_t *obj = _baGetPtrByte (buf, num_bit / 8);
    uint8_t mask = _baGetMaskBitFromByte (num_bit % 8);

    *obj |= mask;
}
static /* inline*/ void _baSetZero (buf_t buf, size_t num_bit) {
    uint8_t *obj = _baGetPtrByte (buf, num_bit / 8);
    uint8_t mask = _baGetMaskBitFromByte (num_bit % 8);

    *obj &= ~mask;
}
static /* inline*/ void _baSetValue (buf_t buf, size_t num_bit, bool val) {
    if (val)
        _baSetOne (buf, num_bit);
    else
        _baSetZero (buf, num_bit);
}

int baSetOne   (BinArray* arr, size_t num_bit) {

    CHECK_PBA (arr);

    if (num_bit > arr->num_bits_) {
        errno = E2BIG;
        return -1;
    }

    _baSetOne (arr->buf_, num_bit);

    return 0;
}
int baSetZero  (BinArray* arr, size_t num_bit) {

    CHECK_PBA (arr);

    if (num_bit > arr->num_bits_) {
        errno = E2BIG;
        return -1;
    }

    _baSetZero (arr->buf_, num_bit);

    return 0;
}
int baSetValue (BinArray* arr, size_t num_bit, bool val) {
    CHECK_PBA (arr);

    if (num_bit > arr->num_bits_) {
        errno = E2BIG;
        return -1;
    }

    _baSetValue (arr->buf_, num_bit, val);

    return 0;
}

// ==============\\
// Find functions -------------------------------------------------------------
// ==============//

ssize_t baFind (BinArray* arr, size_t begin, ssize_t len, bool val) {
    // 64 bits CPU

    if (val)
        return baFindOne (arr, 0, -1);
    else
        return baFindZero (arr, 0, -1);
}

// Find One -------------------------------------------------------------------

static /* inline*/ int8_t _baFindBitOneInQWord (uint64_t qword) {
    return ffsll (qword) - 1;
}

ssize_t baFindOne  (BinArray* arr, size_t begin, ssize_t len) {
    if (baCheckCalcArg (arr, begin, &len) == false)
        return -1;

    ssize_t pos = 0;
    uint64_t* buf = ((uint64_t*) arr->buf_) + begin / 64;
    const uint8_t rshift = begin % 64;

    if (rshift != 0) {
        uint64_t first_qword = (*buf) >> rshift;

        pos = _baFindBitOneInQWord (first_qword);

        if (rshift + len <= 64) {       // |-**- ----|---- ----|
            if (pos != -1 && pos < len)
                return begin + pos;
            
            return -1;
        } 
        
        if (pos != -1)
            return begin + pos;

        ++buf;
        len -= 64 - rshift;
        pos = begin + (64 - rshift);
    } else {
        pos = begin;
    }
    
    // Here we know that the buf starts from the beginning

    uint32_t num_u64 = baGetSizeArrayIn8Byte (len), i = 0;
    
    ssize_t add_pos = -1;
    for (i = 0; i < num_u64; ++i)
        if (buf[i]) {
            add_pos = _baFindBitOneInQWord (buf[i]);
            break;
        }

    if (add_pos != -1 && add_pos < len)
        return 64 * i + pos + add_pos;
    
    return -1;
}

// Find Zero ------------------------------------------------------------------

static /* inline*/ int8_t _baFindBitZeroInQWord (uint64_t qword) {
   return _baFindBitOneInQWord ((uint64_t) ~qword);
}

ssize_t baFindZero (BinArray* arr, size_t begin, ssize_t len) {
    if (baCheckCalcArg (arr, begin, &len) == false)
        return -1;

    ssize_t pos = 0;
    uint64_t* buf = ((uint64_t*) arr->buf_) + begin / 64;
    const uint8_t rshift = begin % 64;

    if (rshift != 0) {
        uint64_t first_qword = ((int64_t) *buf) >> rshift;

        pos = _baFindBitZeroInQWord (first_qword);

        if (rshift + len <= 64) {       // |-**- ----|---- ----|
            if (pos != -1 && pos < len)
                return begin + pos;
            
            return -1;
        } 
        
        if (pos != -1)
            return begin + pos;

        ++buf;
        len -= 64 - rshift;
        pos = begin + (64 - rshift);
    } else {
        pos = begin;
    }
    
    // Here we know that the buf starts from the beginning

    uint32_t num_u64 = baGetSizeArrayIn8Byte (len), i = 0;
    
    ssize_t add_pos = -1;
    for (i = 0; i < num_u64; ++i)
        if (buf[i] != 0xFFFFFFFFFFFFFFFF) {
            add_pos = _baFindBitZeroInQWord (buf[i]);
            break;
        }

    if (add_pos != -1 && add_pos < len)
        return 64 * i + pos + add_pos;
    
    return -1;
}

// ==============\\
// Fill functions -------------------------------------------------------------
// ==============//

// len + rshift <= 64
static uint64_t _baGetMaskFirtsQWord (uint8_t rshift, uint8_t len) {
    return (uint64_t) (0xFFFFFFFFFFFFFFFFULL >> (64 - len)) << rshift;
}
static uint64_t _baGetMaskLastQWord (uint8_t len) {
    return (uint64_t) (0xFFFFFFFFFFFFFFFFULL >> (64 - len));
}

int baFillOne  (BinArray* arr, size_t begin, ssize_t len) {
    if (baCheckCalcArg (arr, begin, &len) == false)
        return -1;

    ssize_t pos = 0;
    uint64_t* buf = ((uint64_t*) arr->buf_) + begin / 64;
    const uint8_t rshift = begin % 64;

    if (rshift + len <= 64) {
        *buf |= _baGetMaskFirtsQWord (rshift, len);
        return 0;
    }

    const size_t rlen = 64 - rshift;

    *buf++ |= _baGetMaskFirtsQWord (rshift, rlen);
    pos += begin + rlen;
    len -= rlen;
    
    // Here we know that the buf starts from the beginning

    ssize_t num_u64 = baGetSizeArrayIn8Byte (len) - 1, i = 0;
    if (num_u64 > 0) {
        memset (buf, 0xFF, 8 * num_u64);
        buf += num_u64;
        len -= 64 * num_u64;
    }

    *buf |= _baGetMaskLastQWord (len);

    return 0;
}
int baFillZero (BinArray* arr, size_t begin, ssize_t len) {
    if (baCheckCalcArg (arr, begin, &len) == false)
        return -1;

    ssize_t pos = 0;
    uint64_t* buf = ((uint64_t*) arr->buf_) + begin / 64;
    const uint8_t rshift = begin % 64;

    if (rshift + len <= 64) {
        *buf &= ~_baGetMaskFirtsQWord (rshift, len);
        return 0;
    }

    const size_t rlen = 64 - rshift;

    *buf++ &= ~_baGetMaskFirtsQWord (rshift, rlen);
    pos += begin + rlen;
    len -= rlen;
    
    // Here we know that the buf starts from the beginning

    ssize_t num_u64 = baGetSizeArrayIn8Byte (len) - 1, i = 0;
    if (num_u64 > 0) {
        memset (buf, 0x00, 8 * num_u64);
        buf += num_u64;
        len -= 64 * num_u64;
    }

    *buf &= ~_baGetMaskLastQWord (len);

    return 0;
}

int baFillZeroFull (BinArray* arr) {
    CHECK_PBA (arr);
    
    // NEED TO OPTIMIZE SPEED!
    memset (arr->buf_, 0x00, 8 * baGetSizeArrayIn8Byte (arr->num_bits_));
    
    return 0;
}
int baFillOneFull (BinArray* arr) {
    CHECK_PBA (arr);

    // NEED TO OPTIMIZE SPEED!
    memset (arr->buf_, 0xFF, 8 * baGetSizeArrayIn8Byte (arr->num_bits_));

    return 0;
}

// ==============\\
// Dump functions -------------------------------------------------------------
// ==============//

int baDumpBuf (BinArray* arr, size_t begin, ssize_t len) {
    if (baCheckCalcArg (arr, begin, &len) == false)
        return -1;

    BinArray* temp = baGetSubArray (arr, begin, len);
    if (temp == NULL)
        return -1;

    baDumpBufFull (temp);

    baDestroy (&temp);

    return 0;
}
int baDumpBufFull (BinArray* arr) {

    CHECK_PBA (arr);

    const size_t num_bits = _baGetNumBits (arr);
    size_t i = 0;

    for (i = 0; i < num_bits; ++i) {
        if (i % 8 == 0 && i != 0)
            printf (" ");

        printf ("%d", _baGetValue (arr->buf_, i));
    }
    printf ("\n");
}

// ================\\
// Foreach function -----------------------------------------------------------
// ================//

int baForeach (BinArray* arr,
             int (*cb) (BinArray* arr, bool element, void* data),
             void* data)
{
    const size_t num_bits = arr->num_bits_;
    for (size_t ibit = 0; ibit < num_bits; ++ibit)
        cb (arr, _baGetValue (arr->buf_, ibit), data);

    return 0;
}

// ================\\
// Invert functions -----------------------------------------------------------
// ================//

int baInvert (BinArray* arr, size_t begin, ssize_t len) {
    if (baCheckCalcArg (arr, begin, &len) == false)
        return -1;

    ssize_t pos = 0;
    uint64_t* buf = ((uint64_t*) arr->buf_) + begin / 64;
    const uint8_t rshift = begin % 64;

    if (rshift + len <= 64) {
        const uint64_t mask = _baGetMaskFirtsQWord (rshift, len);
        *buf = ((uint64_t) ~(*buf & mask)) & mask;
        return 0;
    }
    
    const size_t rlen = 64 - rshift;

    uint64_t mask = _baGetMaskFirtsQWord (rshift, rlen);
    *buf++ = (~(uint64_t)(*buf & mask)) & mask;
    
    pos += begin + rlen;
    len -= rlen;
    
    // Here we know that the buf starts from the beginning

    ssize_t num_u64 = baGetSizeArrayIn8Byte (len) - 1, i = 0;
    if (num_u64 > 0) {
        for (ssize_t i = 0; i < num_u64; ++i) {
            *buf++ = ~((uint64_t) *buf);
            len -= 64 * num_u64;
        }
    }

    mask = _baGetMaskLastQWord (len);
    *buf = ((uint64_t) ~(*buf & mask)) & mask;

    return 0;
}
BinArray* baGetInvert (BinArray* arr) {
    if (baCheckPtr (arr) == false) {
        errno = EINVAL;
        return NULL;
    }

    BinArray *inv_arr = baCreate (arr->num_bits_);
    if (inv_arr == NULL)
        return NULL;
    
    // Ivert code ------------------------------------

    const unsigned bits_in_8bytes = 8 * 8;  // So that the compiler can see that we are 
                                            // dividing and taking the remainder of the
                                            // division of one number.
    
    size_t  num_u64 = arr->num_bits_ / bits_in_8bytes;
    uint8_t num_u1  = arr->num_bits_ % bits_in_8bytes;
    uint8_t num_u8  = baBits2Bytes (num_u1);
    
    // Invert bloks of 8 bytes ----------------------
    uint64_t* buf_u64 = (uint64_t*) arr->buf_;
    uint64_t* inv_buf_u64 = (uint64_t*) inv_arr->buf_;
    if (num_u64 != 0)
        while (num_u64--)
            *inv_buf_u64++ = ~*buf_u64++;

    // Invert bloks of 1 bytes ----------------------
    uint8_t* buf_u8 = (uint8_t*) buf_u64;
    uint8_t* inv_buf_u8 = (uint8_t*) inv_buf_u64;
    if (num_u8 != 0) 
        while (num_u8--)
            *inv_buf_u8++ = ~*buf_u8++;

    // -----------------------------------------------

    return inv_arr;
}