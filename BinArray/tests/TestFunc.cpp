#include "TestFunc.hpp"

#include <gtest/gtest.h>
#include <random>
#include <cassert>

struct BinArray* GetArrayBA (size_t num) {
    return new struct BinArray[num];
}

void RandRetainArrayBA (struct BinArray arrs[], size_t num, 
                        size_t size_from, size_t size_to)
{
    std::random_device rd;
    std::mt19937_64 gen (rd ());

    using uni_dest = std::uniform_int_distribution <>;
    uni_dest uint32_rand (size_from, size_to);

    for (size_t i = 0; i < num; ++i) {
        ASSERT_EQ (baRetain (&arrs[i], uint32_rand (gen)), 0);
    }
}

void RandResizeArrayBA (struct BinArray arrs[], size_t num, 
                        size_t size_from, size_t size_to)
{
    std::random_device rd;
    std::mt19937_64 gen (rd ());

    using uni_dest = std::uniform_int_distribution <>;
    uni_dest uint32_rand (size_from, size_to);

    for (size_t i = 0; i < num; ++i) {
        ASSERT_EQ (baResize (&arrs[i], uint32_rand (gen)), 0);
    }
}

void ReleaseArrayBA (struct BinArray arrs[], size_t num) {
    for (size_t i = 0; i < num; ++i) {
        ASSERT_EQ (baRelease (&arrs[i]), 0);
    }
}

void FillOneArrayBA (struct BinArray arrs[], size_t num) {
    for (size_t i = 0; i < num; ++i) {
        ASSERT_EQ (baFill_One (arrs[i]), 0);
    }
}

void FillZeroArrayBA (struct BinArray arrs[], size_t num) {
    for (size_t i = 0; i < num; ++i) {
        ASSERT_EQ (baFill_Zero (arrs[i]), 0);
    }
}

void CheckOneArrayBA (struct BinArray arrs[], size_t num) {
    for (size_t i = 0; i < num; ++i) {
        struct BinArray arr = arrs[i];
        
        const size_t num_bytes = arr.num_bits_ / 8;
        for (size_t j = 0; j < num_bytes; ++j) {
            ASSERT_EQ (arr.buf_[j], 0xFF);
        }
    }
}

void CheckZeroArrayBA (struct BinArray arrs[], size_t num) {
    for (size_t i = 0; i < num; ++i) {
        struct BinArray arr = arrs[i];
        
        const size_t num_bytes = arr.num_bits_ / 8;
        for (size_t j = 0; j < num_bytes; ++j) {
            ASSERT_EQ (arr.buf_[j], 0x00);
        }
    }
}

void CheckOneFromGetValueArrayBA (struct BinArray arrs[], size_t num) {
    for (size_t i = 0; i < num; ++i) {
        struct BinArray arr = arrs[i];
        
        const size_t num_bits = arr.num_bits_;
        for (size_t j = 0; j < num_bits; ++j) {
            ASSERT_EQ (baGetValue (arr, j), 1);
        }
    }
}

void CheckZeroFromGetValueArrayBA (struct BinArray arrs[], size_t num) {
    for (size_t i = 0; i < num; ++i) {
        struct BinArray arr = arrs[i];
        
        const size_t num_bits = arr.num_bits_;
        for (size_t j = 0; j < num_bits; ++j) {
            ASSERT_EQ (baGetValue (arr, j), 0);
        }
    }
}

void CheckSetAndGetOneArrayBA (struct BinArray arrs[], size_t num) {
    for (size_t i = 0; i < num; ++i) {
        struct BinArray arr = arrs[i];
        
        const size_t num_bits = arr.num_bits_;
        for (size_t j = 0; j < num_bits; ++j) {
            ASSERT_EQ (baSetValue (arr, j, 1), 0);
            ASSERT_EQ (baGetValue (arr, j   ), 1);
        }
    }
}

void CheckSetAndGetZeroArrayBA (struct BinArray arrs[], size_t num) {
    for (size_t i = 0; i < num; ++i) {
        struct BinArray arr = arrs[i];
        
        const size_t num_bits = arr.num_bits_;
        for (size_t j = 0; j < num_bits; ++j) {
            ASSERT_EQ (baSetValue (arr, j, 0), 0);
            ASSERT_EQ (baGetValue (arr, j   ), 0);
        }
    }
}