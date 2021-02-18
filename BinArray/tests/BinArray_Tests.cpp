#include <gtest/gtest.h>
#include <random>

#include "../BinArray.h"
#include "TestFunc.hpp"

const size_t num_arr_g = 5000                / 1;
const size_t max_size_arr_g = UINT16_MAX * 4 / 1;


TEST (BIN_ARRAY, Error_Create_Resize_Release) { 
    ASSERT_TRUE (baCreate (0) == NULL);
    ASSERT_TRUE (baCreate (0) == NULL);

    BinArray* arr = baCreate (1);
    ASSERT_TRUE (baResize (arr,  0) == -1);
    ASSERT_TRUE (baResize (NULL, 0) == -1);
    baDestroy (&arr);

    ASSERT_TRUE (baDestroy (NULL) == -1);
}

TEST (BIN_ARRAY, True_Create_Resize_Release) {
    
    const float koef_num_arr = 50;

    const size_t num_arr = koef_num_arr * num_arr_g;   
    
    BinArray** arrs = nullptr;
    // --------------------------------------------------

    arrs = GetArrayBA (num_arr);
    RandRetainArrayBA (arrs, num_arr, 1, max_size_arr_g);
    ReleaseArrayBA (arrs, num_arr);

    // --------------------------------------------------

    arrs = GetArrayBA (num_arr);
    RandRetainArrayBA (arrs, num_arr, 1, max_size_arr_g);
    RandResizeArrayBA (arrs, num_arr, 1, max_size_arr_g);
    ReleaseArrayBA (arrs, num_arr);

    // --------------------------------------------------

    delete [] arrs;
}

TEST (BIN_ARRAY, GetCLone) {
    const float koef_num_arr = 0.02;

    const size_t num_arr = koef_num_arr * num_arr_g;

    BinArray** arrs = nullptr;    
    // --------------------------------------------------

    arrs = GetArrayBA (num_arr);
    RandRetainArrayBA (arrs, num_arr, 1, max_size_arr_g);

    // --------------------------------------------------

    FillRandArrayBA (arrs, num_arr);
    for (size_t iarr = 0; iarr < num_arr; ++iarr) {
        BinArray* arr = arrs[iarr];
        
        BinArray* inv_arr = baGetClone (arr);

        for (int i = 0; i < baGetNumBits (arr); i++) {
            bool elem = baGetValue (arr, i);
            bool inv_elem = baGetValue (inv_arr, i);

            ASSERT_TRUE (elem == inv_elem);
        }
    }
    
    // --------------------------------------------------

    delete [] arrs;
}

TEST (BIN_ARRAY, Filling_And_Getting_One_And_Zero) {

    const float koef_num_arr = 0.05f;

    const size_t num_arr = koef_num_arr * num_arr_g;

    BinArray** arrs = nullptr;

    // --------------------------------------------------

    arrs = GetArrayBA (num_arr);
    RandRetainArrayBA (arrs, num_arr, 1, max_size_arr_g);

    FillOneArrayBA (arrs, num_arr);
    CheckOneArrayBA (arrs, num_arr);

    ReleaseArrayBA (arrs, num_arr);

    // --------------------------------------------------

    arrs = GetArrayBA (num_arr);
    RandRetainArrayBA (arrs, num_arr, 1, max_size_arr_g);

    FillZeroArrayBA (arrs, num_arr);
    CheckZeroArrayBA (arrs, num_arr);

    ReleaseArrayBA (arrs, num_arr);

    // --------------------------------------------------

    delete [] arrs;
}

TEST (BIN_ARRAY, Setting_And_GettingOne_OrZero) {

    const float koef_num_arr = 0.005f;

    const size_t num_arr = koef_num_arr * num_arr_g;   

    BinArray** arrs = nullptr;

    // --------------------------------------------------

    arrs = GetArrayBA (num_arr);
    RandRetainArrayBA (arrs, num_arr, 1, max_size_arr_g);

    CheckSetAndGetOneArrayBA (arrs, num_arr);

    ReleaseArrayBA (arrs, num_arr);

    // --------------------------------------------------

    arrs = GetArrayBA (num_arr);
    RandRetainArrayBA (arrs, num_arr, 1, max_size_arr_g);

    CheckSetAndGetZeroArrayBA (arrs, num_arr);

    ReleaseArrayBA (arrs, num_arr);

    // --------------------------------------------------

    delete [] arrs;
}

TEST (BIN_ARRAY, Tetsting_Find) {
    const float koef_num_arr = 0.02;
    const float koef_num_check = 0.02;

    const size_t num_arr = koef_num_arr * num_arr_g;
    
    BinArray** arrs = nullptr;

    // --------------------------------------------------
    
    arrs = GetArrayBA (num_arr);
    RandRetainArrayBA (arrs, num_arr, 1, max_size_arr_g);

    // --------------------------------------------------

    CheckFindOne  (arrs, num_arr, koef_num_check);
    CheckFindZero (arrs, num_arr, koef_num_check);
    CheckFind     (arrs, num_arr, koef_num_check);
    
    // --------------------------------------------------

    delete [] arrs;
}

static int find_max (BinArray* arr, bool el, void* data) {
    bool max = *(bool*) data;
    if (el > max)
        *(bool*) data = el;

    return 0;
}

static int find_min (BinArray* arr, bool el, void* data) {
    bool min = *(bool*) data;
    if (el < min)
        *(bool*) data = el;

    return 0;
}

TEST (BIN_ARRAY, foreach) {

    const float koef_num_arr = 0.1;

    const size_t num_arr = koef_num_arr * num_arr_g;

    BinArray** arrs = nullptr;

    // --------------------------------------------------

    arrs = GetArrayBA (num_arr);
    RandRetainArrayBA (arrs, num_arr, 1, max_size_arr_g);

    // --------------------------------------------------

    for (int i = 0; i < num_arr; ++i) {
        bool max = false;
        foreach (arrs[i], find_max, &max);

        if (baFindOne (arrs[i]) == -1) {
            ASSERT_FALSE (max);
        } else {
            ASSERT_TRUE (max);
        }
    }

    // --------------------------------------------------

    for (int i = 0; i < num_arr; ++i) {
        bool min = true;
        foreach (arrs[i], find_min, &min);

        if (baFindZero (arrs[i]) == -1) {
            ASSERT_TRUE (min);
        } else {
            ASSERT_FALSE (min);
        }
    }

    // --------------------------------------------------

    ReleaseArrayBA (arrs, num_arr);

    delete [] arrs;
}

TEST (BIN_ARRAY, Find) {
    const float koef_num_arr = 0.01;
    const float koef_num_check = 0.02;

    const size_t num_arr = koef_num_arr * num_arr_g;

    BinArray** arrs = nullptr;

    // --------------------------------------------------
    
    arrs = GetArrayBA (num_arr);
    RandRetainArrayBA (arrs, num_arr, 1, max_size_arr_g);

    // --------------------------------------------------

    CheckFindOne  (arrs, num_arr, koef_num_check);
    CheckFindZero (arrs, num_arr, koef_num_check);
    CheckFind     (arrs, num_arr, koef_num_check);
    
    // --------------------------------------------------

    delete [] arrs;
}

TEST (BIN_ARRAY, Invert_And_GetInvert) {
    const float koef_num_arr = 0.01;

    const size_t num_arr = koef_num_arr * num_arr_g;

    BinArray** arrs = nullptr;
    
    // --------------------------------------------------
    
    arrs = GetArrayBA (num_arr);
    RandRetainArrayBA (arrs, num_arr, 1, max_size_arr_g);

    // Invert -------------------------------------------

    FillRandArrayBA (arrs, num_arr);
    for (size_t iarr = 0; iarr < num_arr; ++iarr) {
        BinArray* arr = arrs[iarr];
        
        BinArray* inv_arr = baGetClone (arr);
        baInvert (inv_arr);

        size_t num_bits = baGetNumBits (arr);
        for (int i = 0; i < num_bits; i++) {
            bool elem = baGetValue (arr, i);
            bool inv_elem = baGetValue (inv_arr, i);

            ASSERT_TRUE (elem != inv_elem);
        }
    }

    // GetInvert ----------------------------------------

    for (size_t iarr = 0; iarr < num_arr; ++iarr) {
        BinArray* arr = arrs[iarr];
        
        BinArray* inv_arr = baGetInvert (arr);

        size_t num_bits = baGetNumBits (arr);
        for (int i = 0; i < num_bits; i++) {
            bool elem = baGetValue (arr, i);
            bool inv_elem = baGetValue (inv_arr, i);

            ASSERT_TRUE (elem != inv_elem);
        }
    }
    
    // --------------------------------------------------

    delete [] arrs;
}