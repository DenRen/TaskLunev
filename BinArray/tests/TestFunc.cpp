#include "TestFunc.hpp"

#include <gtest/gtest.h>
#include <random>
#include <cassert>

void ReadFromStdoutToBuf (char* buf, uint8_t size) {
    fflush (stdout);
    fseek (stdout, 0, SEEK_SET);
    int ret = fread (buf, sizeof (char), size, stdout);
    fseek (stdout, 0, SEEK_SET);
}

BinArray** GetArrayBA (size_t num) {
    return new BinArray*[num];
}

void RandCreateArrayBA (BinArray* arrs[], size_t num, 
                        size_t size_from, size_t size_to)
{
    std::random_device rd;
    std::mt19937_64 gen (rd ());

    using uni_dest = std::uniform_int_distribution <>;
    uni_dest uint32_rand (size_from, size_to);

    for (size_t i = 0; i < num; ++i) {
        arrs[i] = baCreate (uint32_rand (gen));
        ASSERT_TRUE (arrs[i] != NULL);
    }
}

void RandResizeArrayBA (BinArray* arrs[], size_t num, 
                        size_t size_from, size_t size_to)
{
    std::random_device rd;
    std::mt19937_64 gen (rd ());

    using uni_dest = std::uniform_int_distribution <>;
    uni_dest uint32_rand (size_from, size_to);

    for (size_t i = 0; i < num; ++i) {
        ASSERT_EQ (baResize (arrs[i], uint32_rand (gen)), 0);
    }
}

void DestroyArrayBA (BinArray* arrs[], size_t num) {
    for (size_t i = 0; i < num; ++i) {
        ASSERT_EQ (baDestroy (&arrs[i]), 0);
    }
}

void FillOneFullArrayBA (BinArray* arrs[], size_t num) {
    for (size_t i = 0; i < num; ++i) {
        ASSERT_EQ (baFillOneFull (arrs[i]), 0);
    }
}

void FillZeroFullArrayBA (BinArray* arrs[], size_t num) {
    for (size_t i = 0; i < num; ++i) {
        ASSERT_EQ (baFillZeroFull (arrs[i]), 0);
    }
}

void FillRandArrayBA (BinArray* arrs[], size_t num) {
    std::random_device rd;
    std::mt19937 gen (rd ());

    using uni_dest = std::uniform_int_distribution <>;
    uni_dest bool_rand (0, 1);
    
    for (size_t iarr = 0; iarr < num; ++iarr) {
        BinArray*arr = arrs[iarr];

        size_t num_bits = baGetNumBits (arr);
        for (size_t i = 0; i < num_bits; ++i)
            baSetValue (arr, i, bool_rand (gen));
    }
}

void CheckFillOneBA (size_t num_bits, size_t begin, ssize_t len) {
    BinArray* arr = baCreate (num_bits);
    ASSERT_TRUE (arr != NULL);

    ASSERT_EQ (baFillZeroFull (arr), 0);
    ASSERT_EQ (baFillOne (arr, begin, len), 0);

    BinArray* result = baGetSubArray (arr, begin, len);
    ASSERT_TRUE (result != NULL);

    CheckOneFullBA (result);

    baDestroy (&result);
    baDestroy (&arr);
}

void CheckOneFullBA (BinArray* arr) {
    size_t num_bits = baGetNumBits (arr);
    for (size_t j = 0; j < num_bits; ++j) {
        ASSERT_EQ (baGetValue (arr, j), 1) << baDumpBufFull (arr);
    }
}

void CheckOneFullArrayBA (BinArray* arrs[], size_t num) {
    for (size_t i = 0; i < num; ++i)
        CheckOneFullBA (arrs[i]);
}

void CheckZeroFullBA (BinArray* arr) {
    size_t num_bits = baGetNumBits (arr);
    for (size_t j = 0; j < num_bits; ++j) {
        ASSERT_EQ (baGetValue (arr, j), 0) << baDumpBufFull (arr);
    }
}

void CheckZeroFullArrayBA (BinArray* arrs[], size_t num) {
    for (size_t i = 0; i < num; ++i)
        CheckZeroFullBA (arrs[i]);
}

void CheckSetAndGetOneArrayBA (BinArray* arrs[], size_t num) {
    for (size_t i = 0; i < num; ++i) {
        BinArray* arr = arrs[i];
        
        size_t num_bits = baGetNumBits (arr);
        for (size_t j = 0; j < num_bits; ++j) {
            ASSERT_EQ (baSetValue (arr, j, 1), 0);
            ASSERT_EQ (baGetValue (arr, j   ), 1);
        }
    }
}

void CheckSetAndGetZeroArrayBA (BinArray* arrs[], size_t num) {
    for (size_t i = 0; i < num; ++i) {
        BinArray* arr = arrs[i];
        
        size_t num_bits = baGetNumBits (arr);
        for (size_t j = 0; j < num_bits; ++j) {
            ASSERT_EQ (baSetValue (arr, j, 0), 0);
            ASSERT_EQ (baGetValue (arr, j   ), 0);
        }
    }
}

void CheckFindOne  (BinArray* arrs[], size_t num, const float koef_num_check) {
    FillZeroFullArrayBA (arrs, num);

    std::random_device rd;
    std::mt19937_64 gen (rd ());

    using uni_dest = std::uniform_int_distribution <>;
    for (int iarr = 0; iarr < num; ++iarr) {

        BinArray* arr = arrs[iarr];
        uni_dest rand_num_bit (0, baGetNumBits (arr) -1);
        size_t num_attempts = koef_num_check * baGetNumBits (arr);

        for (int i = 0; i < num_attempts; ++i) {
            size_t num_bit = rand_num_bit (gen);
            baSetOne (arr, num_bit);

            int64_t num_from_find = baFindOne (arr, 0, -1);
            ASSERT_EQ (num_bit, num_from_find);

            baSetZero (arr, num_bit);
        }
    }
}

void CheckFindZero (BinArray* arrs[], size_t num, const float koef_num_check) {

    std::random_device rd;
    std::mt19937_64 gen (rd ());

    using uni_dest = std::uniform_int_distribution <>;

    // ------------------------------------------------------------

    FillZeroFullArrayBA (arrs, num);

    for (int iarr = 0; iarr < num; ++iarr) {

        BinArray* arr = arrs[iarr];
        uni_dest rand_num_bit (0, baGetNumBits (arr) -1);
        size_t num_attempts = koef_num_check * baGetNumBits (arr);

        for (int i = 0; i < num_attempts; ++i) {
            size_t num_bit = rand_num_bit (gen);
            baSetOne (arr, num_bit);

            int64_t num_from_find = baFindOne (arr, 0, -1);
            ASSERT_EQ (num_bit, num_from_find);

            baSetZero (arr, num_bit);
        }
    }

    // ------------------------------------------------------------

    FillOneFullArrayBA (arrs, num);

    for (int iarr = 0; iarr < num; ++iarr) {

        BinArray* arr = arrs[iarr];
        uni_dest rand_num_bit (0, baGetNumBits (arr) -1);
        size_t num_attempts = koef_num_check * baGetNumBits (arr);

        for (int i = 0; i < num_attempts; ++i) {
            size_t num_bit = rand_num_bit (gen);
            baSetZero (arr, num_bit);

            int64_t num_from_find = baFindZero (arr, 0, -1);
            ASSERT_EQ (num_bit, num_from_find) << "Zero";

            baSetOne (arr, num_bit);
        }
    }
}

void CheckFind (BinArray* arrs[], size_t num, const float koef_num_check) {
    FillOneFullArrayBA (arrs, num);

    std::random_device rd;
    std::mt19937_64 gen (rd ());

    using uni_dest = std::uniform_int_distribution <>;
    for (int iarr = 0; iarr < num; ++iarr) {

        BinArray* arr = arrs[iarr];
        uni_dest rand_num_bit (0, baGetNumBits (arr) -1);
        size_t num_attempts = koef_num_check * baGetNumBits (arr);

        for (int i = 0; i < num_attempts; ++i) {
            size_t num_bit = rand_num_bit (gen);
            baSetZero (arr, num_bit);

            int64_t num_from_find = baFind (arr, 0, -1, 0);
            ASSERT_EQ (num_bit, num_from_find)  
            << baGetNumBits (arr) << " " << num_bit << " " << num_from_find;

            baSetOne (arr, num_bit);
        }
    }
}