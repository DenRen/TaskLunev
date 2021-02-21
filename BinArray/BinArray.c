#include "BinArray.h"
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

// ==========\\
// Main types -----------------------------------------------------------------
// ==========//

typedef uint8_t* buf_t;

typedef struct bin_array {
    buf_t buf_;
    size_t num_bits_;
} BinArray;

// You cannot call baCreate and baDestroy for proxy bin array.
// It's special type to create a dummy bin subarray   
typedef struct proxy_bin_array {
    BinArray arr;
} proxyBinArray;

// ===================\\
// Secondary functions --------------------------------------------------------
// ===================//

size_t baBits2Bytes (size_t num_bits) {
    return num_bits / 8 + (num_bits % 8 != 0);
}
inline size_t _baGetNumBits (BinArray* arr) {
    return arr->num_bits_;
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

// Может быть уже нет смысла в baCheckArr?

inline bool baCheckArr (BinArray arr) {
    return (arr.buf_ != NULL) && (arr.buf_ != 0);
}

inline bool baCheckPtr (BinArray* arr) {
    return arr != NULL && baCheckArr (*arr);
}

// arr - correct pointer to an existing array
inline bool baCheckSubVector (BinArray* arr, size_t begin, ssize_t len) {
    if (len == -1)
        return begin < _baGetNumBits (arr);
    else if (len > 0)
        return begin + len < _baGetNumBits (arr);
    else 
        return false;
}

inline bool baCheckPtr_SV (BinArray* arr, size_t begin, ssize_t len) {
    return baCheckPtr (arr) && baCheckSubVector (arr, begin, len);
}

// Effctive check and calculate input args
inline bool baCheckCalcArg (BinArray* arr, size_t begin, ssize_t* len) {
    
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

    int num_bytes = baBits2Bytes (num_bits);

    BinArray* arr = (BinArray*) calloc (1, sizeof (BinArray));
    if (arr == NULL)
        return NULL;

    if ((arr->buf_ = (uint8_t *) calloc (sizeof (uint8_t), num_bytes)) == NULL) {
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
    
    uint8_t *new_buf = (uint8_t *) reallocarray (arr->buf_, 
                                                 baBits2Bytes (new_num_bits),
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

    memcpy (clone->buf_, arr->buf_, baBits2Bytes (clone->num_bits_));

    return clone;
}

BinArray *baGetSubArray (BinArray* arr, size_t begin, ssize_t len) {
    if (baCheckCalcArg (arr, begin, &len) == false) 
        return NULL;

    BinArray* clone = baCreate (len);
    if (clone == NULL)
        return NULL;

    uint8_t* clone_buf = clone->buf_;
    uint8_t* buf = arr->buf_ + begin / 8;
    const uint8_t lshift = begin % 8;
    if (lshift == 0) {
        memcpy (clone->buf_, buf, baBits2Bytes (len));
    } else {
        union reg_u16 {
            uint8_t  bytes[2];
            uint16_t word;
        };

        clone_buf[0] = buf[0] << lshift;
        
        ssize_t num_rest_bits = len - (8 - lshift); // rest - остальные
        if (num_rest_bits <= 0)
            return clone;

        union reg_u16 reg;
        const size_t num_full_bytes = num_rest_bits / 8;
        for (size_t i = 1; i < num_full_bytes + 1; ++i) {
            reg.bytes[0] = buf[i];
            reg.bytes[1] = 0;

            reg.word <<= lshift;

            clone_buf[i - 0]  = reg.bytes[0];
            clone_buf[i - 1] |= reg.bytes[1];
        }

        num_rest_bits %= 8;
        if (num_rest_bits) {
            reg.bytes[0] = buf[num_full_bytes + 1];
            reg.bytes[1] = 0;

            reg.word <<= lshift;

            clone_buf[num_full_bytes] |= reg.bytes[1];

            if (num_rest_bits > lshift)
                clone_buf[num_full_bytes + 1]  = reg.bytes[0];
        }
    }

    return clone;
}

size_t baGetNumBits (BinArray* arr) {
    return _baGetNumBits (arr);
}

// =============================\\
// Getters and setters functions ----------------------------------------------
// =============================//

// Without checking for correctness (for speed)
static inline bool _baGetValue (buf_t buf, size_t num_bit) {
    uint8_t obj = *(buf + num_bit / 8);

    return (obj & (1U << (7 - num_bit % 8)));
}

int baGetValue (BinArray *arr, size_t num_bit) {

    CHECK_PBA (arr);

    if (num_bit > arr->num_bits_) {
        errno = E2BIG;
        return -1;
    }

    return _baGetValue (arr->buf_, num_bit);
}

// Without checking for correctness (for speed)
static inline void _baSetValue (buf_t buf, size_t num_bit, bool val) {
    uint8_t *obj = buf + num_bit / 8;
    uint8_t mask = 1U << (7 - num_bit % 8);

    if (val == 1)
        *obj |= mask;
    else
        *obj &= ~mask;
}

static inline void _baSetOne (buf_t buf, size_t num_bit) {
    uint8_t *obj = buf + num_bit / 8;
    uint8_t mask = 1U << (7 - num_bit % 8);

    *obj |= mask;
}

static inline void _baSetZero (buf_t buf, size_t num_bit) {
    uint8_t *obj = buf + num_bit / 8;
    uint8_t mask = 1U << (7 - num_bit % 8);

    *obj &= ~mask;
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

static inline int8_t _baFindBiteOneInByte (uint8_t byte) {
    uint8_t mask = 1U << 7;

    for (int i = 0; i < 8; ++i)
        if (byte & (mask >> i))
            return i;

    return -1;
}

static inline int8_t _baFindBiteOneInQWord (uint64_t qword) {
    const uint8_t *buf_u8 = (uint8_t *) &qword;

    int8_t path = 0;
    for (uint8_t i = 0; i < 8; ++i)
        if (buf_u8[i])
        {
            path += i * 8;

            return path + _baFindBiteOneInByte (buf_u8[i]);
        }
    return -1;
}

// arr - is correct array
static inline ssize_t _baFindOneInBytes  (proxyBinArray prx_arr) {
    const uint64_t *buf_u64 = (uint64_t*) prx_arr.arr.buf_;
    const size_t num_bits = prx_arr.arr.num_bits_; // For speed

    size_t num_u64 = num_bits / (8 * 8);
    size_t i = 0, path = 0;

    for (; i < num_u64; ++i)
        if (buf_u64[i])
            break;

    path = i * 64;

    if (i == num_u64) {
        const int num_u8 = baBits2Bytes ((num_bits - 64 * num_u64));
        const uint8_t* buf_u8 = (const uint8_t*) (buf_u64 + i);

        for (i = 0; i < num_u8; ++i)
            if (buf_u8[i]) { // Equal  if (buf_u8[i])
                path += i * 8;

                uint8_t path_bit = _baFindBiteOneInByte(buf_u8[i]);

                if (path + path_bit >= num_bits)
                    return -1;
                else
                    return path + path_bit;
            }

        return -1;
    }
    else
    {
        return path + _baFindBiteOneInQWord (buf_u64[i]);
    }
}

ssize_t baFindOne  (BinArray* arr, size_t begin, ssize_t len) {
    if (baCheckCalcArg (arr, begin, &len) == false)
        return -1;

    uint8_t* buf = arr->buf_ + begin / 8;
    const uint8_t lshift = begin % 8;
    
    if (lshift + len < 8) {
        const uint8_t mask = ((uint8_t) (0xFF << (8 - len)) >> lshift);
        uint8_t first_u8 = buf[0] & mask;

        if (first_u8)
            return begin - lshift + _baFindBiteOneInByte (first_u8);
        else
            return -1;
    } else {
        uint8_t first_u8 = buf[0] & ((uint8_t) 0xFF >> lshift);

        if (first_u8)
            return begin - lshift + _baFindBiteOneInByte (first_u8);
        
        ++buf;
        len -= 8 - lshift;

        /* g++ doesn't compile like that
        proxyBinArray prx_arr = {
            .arr.buf_ = buf,
            .arr.num_bits_ = len
        };
        */

        proxyBinArray prx_arr;
        prx_arr.arr.buf_ = buf;
        prx_arr.arr.num_bits_ = len;

        ssize_t path = _baFindOneInBytes (prx_arr);
        if (path != -1)
            return begin - lshift + 8 + path;
        else
            return -1;
    }
}

static inline int8_t _baFindBiteZeroInByte (uint8_t byte) {
    uint8_t mask = 1U << 7;

    for (int i = 0; i < 8; ++i)
        if ((byte & (mask >> i)) == 0)
            return i;
    
    return -1;
}

static inline int8_t _baFindBiteZeroInQWord (uint64_t qword) {
    const uint8_t *buf_u8 = (uint8_t *) &qword;

    int8_t path = 0;
    for (uint8_t i = 0; i < 8; ++i)
        if (buf_u8[i] != 0xFF)
        {
            path += i * 8;

            return path + _baFindBiteZeroInByte (buf_u8[i]);
        }
    return -1;
}

// arr - is correct array
static inline ssize_t _baFindZeroInBytes  (proxyBinArray prx_arr) {
    const uint64_t *buf_u64 = (uint64_t*) prx_arr.arr.buf_;
    const size_t num_bits = prx_arr.arr.num_bits_; // For speed

    size_t num_u64 = num_bits / (8 * 8);
    size_t i = 0, path = 0;

    for (; i < num_u64; ++i)
        if (buf_u64[i] != 0xFFFFFFFFFFFFFFFF)
            break;

    path = i * 64;

    if (i == num_u64) {
        const int num_u8 = baBits2Bytes ((num_bits - 64 * num_u64));
        const uint8_t* buf_u8 = (const uint8_t*) (buf_u64 + i);

        for (i = 0; i < num_u8; ++i)
            if (buf_u8[i] != 0xFF) {
                path += i * 8;

                uint8_t path_bit = _baFindBiteZeroInByte(buf_u8[i]);

                if (path + path_bit >= num_bits)
                    return -1;
                else
                    return path + path_bit;
            }

        return -1;
    }
    else
    {
        return path + _baFindBiteZeroInQWord (buf_u64[i]);
    }
}

ssize_t baFindZero (BinArray* arr, size_t begin, ssize_t len) {
    if (baCheckCalcArg (arr, begin, &len) == false)
        return -1;

    uint8_t* buf = arr->buf_ + begin / 8;
    const uint8_t lshift = begin % 8;
    
    if (lshift + len < 8) {
        const uint8_t mask = ((uint8_t) (0xFF << (8 - len)) >> lshift);
        uint8_t first_u8 = buf[0] & mask;
        if (first_u8 != 0xFF)
            return begin - lshift + _baFindBiteZeroInByte (first_u8);
        else
            return -1;
    } else {
        uint8_t first_u8 = buf[0] | ~((uint8_t) 0xFF >> lshift);

        if (first_u8 != 0xFF) 
            return begin - lshift + _baFindBiteZeroInByte (first_u8);
        
        ++buf;
        len -= 8 - lshift;

        /* g++ doesn't compile like that
        proxyBinArray prx_arr = {
            .arr.buf_ = buf,
            .arr.num_bits_ = len
        };
        */

        proxyBinArray prx_arr;
        prx_arr.arr.buf_ = buf;
        prx_arr.arr.num_bits_ = len;
        
        ssize_t path = _baFindZeroInBytes (prx_arr);
        if (path != -1)
            return begin - lshift + 8 + path;
        else
            return -1;
    }
}

// ==============\\
// Fill functions -------------------------------------------------------------
// ==============//

int baFillOne  (BinArray* arr, size_t begin, ssize_t len) {
    if (baCheckCalcArg (arr, begin, &len) == false)
        return -1;

    uint8_t* buf = arr->buf_ + begin / 8;
    const uint8_t lshift = begin % 8;
    
    if (lshift + len < 8) { // |---- -**-|---- --...
        *buf |= ((uint8_t)(0xFF << (8 - len))) >> lshift;
    } else {

        *buf++ |= (uint8_t) 0xFF >> lshift;

        len -= 8 - lshift;
        size_t num_full_bytes = len / 8;
        uint8_t num_rest_bits  = len % 8;

        memset (buf, 0xFF, num_full_bytes);

        if (num_rest_bits)
            *(buf + num_full_bytes) |= 0xFF << (8 - num_rest_bits);
    }

    return 0;
}
int baFillZero (BinArray* arr, size_t begin, ssize_t len) {
    if (baCheckCalcArg (arr, begin, &len) == false)
        return -1;

    uint8_t* buf = arr->buf_ + begin / 8;
    const uint8_t lshift = begin % 8;
    
    if (lshift + len < 8) { // |---- -**-|---- --...
        *buf &= ~(((uint8_t)(0xFF << (8 - len))) >> lshift);
    } else {

        *buf++ &= ~((uint8_t) 0xFF >> lshift);

        len -= 8 - lshift;
        size_t num_full_bytes = len / 8;
        uint8_t num_rest_bits  = len % 8;

        memset (buf, 0x00, num_full_bytes);
        if (num_rest_bits)
            *(buf + num_full_bytes) &= ~(0xFF << (8 - num_rest_bits));
    }

    return 0;
}

int baFillZeroFull (BinArray* arr) {
    CHECK_PBA (arr);

    memset (arr->buf_, 0x00, baBits2Bytes (arr->num_bits_));

    return 0;
}
int baFillOneFull (BinArray* arr) {
    CHECK_PBA (arr);

    memset (arr->buf_, 0xFF, baBits2Bytes (arr->num_bits_));

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

    const size_t num_full_bytes = _baGetNumBits (arr) / 8;
    const size_t num_rest_bits  = _baGetNumBits (arr) % 8;

    bprint (arr->buf_, num_full_bytes);
    
    if (num_rest_bits) {
        uint8_t rest_byte = arr->buf_[num_full_bytes]; 
        for (int i = 0; i < num_rest_bits; ++i)
            printf ("%d", (rest_byte & ((uint8_t) (1U << 7) >> i)) != 0);
    }

    printf ("\n");

    return 0;
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

int baInvert (BinArray* arr) {
    CHECK_PBA (arr);

    const unsigned bits_in_8bytes = 8 * 8;  // So that the compiler can see that we are 
                                            // dividing and taking the remainder of the
                                            // division of one number.
    
    size_t  num_u64 = arr->num_bits_ / bits_in_8bytes;
    uint8_t num_u1  = arr->num_bits_ % bits_in_8bytes;
    uint8_t num_u8  = baBits2Bytes (num_u1);

    // Invert bloks of 8 bytes -----------------------
    uint64_t* buf_u64 = (uint64_t*) arr->buf_;
    
    if (num_u64 != 0)
        while (num_u64--)
            *buf_u64++ = ~*buf_u64;

    // Invert bloks of 1 bytes -----------------------
    uint8_t* buf_u8 = (uint8_t*) buf_u64;
    if (num_u8 != 0) 
        while (num_u8--)
            *buf_u8++ = ~*buf_u8;
    
    // -----------------------------------------------

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