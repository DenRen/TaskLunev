#include "TestFunc.hpp"

#include <gtest/gtest.h>
#include <random>
#include <cassert>

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

void FillOneArrayBA (BinArray* arrs[], size_t num) {
    for (size_t i = 0; i < num; ++i) {
        ASSERT_EQ (baFillOne (arrs[i]), 0);
    }
}

void FillZeroArrayBA (BinArray* arrs[], size_t num) {
    for (size_t i = 0; i < num; ++i) {
        ASSERT_EQ (baFillZero (arrs[i]), 0);
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

void CheckOneArrayBA (BinArray* arrs[], size_t num) {
    for (size_t i = 0; i < num; ++i) {
        BinArray* arr = arrs[i];
        
        size_t num_bits = baGetNumBits (arr);
        for (size_t j = 0; j < num_bits; ++j) {
            ASSERT_EQ (baGetValue (arr, j), 1);
        }
    }
}

void CheckZeroArrayBA (BinArray* arrs[], size_t num) {
    for (size_t i = 0; i < num; ++i) {
        BinArray*arr = arrs[i];
        
        size_t num_bits = baGetNumBits (arr);
        for (size_t j = 0; j < num_bits; ++j) {
            ASSERT_EQ (baGetValue (arr, j), 0);
        }
    }
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
    FillZeroArrayBA (arrs, num);

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

            int64_t num_from_find = baFindOne (arr);
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

    FillZeroArrayBA (arrs, num);

    for (int iarr = 0; iarr < num; ++iarr) {

        BinArray* arr = arrs[iarr];
        uni_dest rand_num_bit (0, baGetNumBits (arr) -1);
        size_t num_attempts = koef_num_check * baGetNumBits (arr);

        for (int i = 0; i < num_attempts; ++i) {
            size_t num_bit = rand_num_bit (gen);
            baSetOne (arr, num_bit);

            int64_t num_from_find = baFindOne (arr);
            ASSERT_EQ (num_bit, num_from_find);

            baSetZero (arr, num_bit);
        }
    }

    // ------------------------------------------------------------

    FillOneArrayBA (arrs, num);

    for (int iarr = 0; iarr < num; ++iarr) {

        BinArray* arr = arrs[iarr];
        uni_dest rand_num_bit (0, baGetNumBits (arr) -1);
        size_t num_attempts = koef_num_check * baGetNumBits (arr);

        for (int i = 0; i < num_attempts; ++i) {
            size_t num_bit = rand_num_bit (gen);
            baSetZero (arr, num_bit);

            int64_t num_from_find = baFindZero (arr);
            ASSERT_EQ (num_bit, num_from_find);

            baSetOne (arr, num_bit);
        }
    }
}

void CheckFind (BinArray* arrs[], size_t num, const float koef_num_check) {
    FillOneArrayBA (arrs, num);

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

            int64_t num_from_find = baFind (arr, 0);
            ASSERT_EQ (num_bit, num_from_find)  
            << baGetNumBits (arr) << " " << num_bit << " " << num_from_find;

            baSetOne (arr, num_bit);
        }
    }
}