#include <gtest/gtest.h>
#include "../BinArray.h"
#include "TestFunc.hpp"

TEST (BIN_ARRAY, Error_Create_Resize_Release) { 
    struct BinArray arr = {};

    ASSERT_TRUE (baRetain (&arr,  0) == -1);
    ASSERT_TRUE (baRetain (NULL,  0) == -1);

    ASSERT_TRUE (baResize (&arr, 0) == -1);
    
    ASSERT_TRUE (baRelease (NULL) == -1);    
}

const size_t num_arr_g = 5000;
const size_t max_size_arr_g = UINT16_MAX * 4;

TEST (BIN_ARRAY, True_Create_Resize_Release) {

    struct BinArray* arrs = nullptr;

    const float koef = 50;

    const size_t num_arr = koef * num_arr_g;   
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

TEST (BIN_ARRAY, Filling_One_And_Zero) {

    struct BinArray* arrs = nullptr;

    const float koef = 0.05f;

    const size_t num_arr = koef * num_arr_g;   

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

TEST (BIN_ARRAY, Getting_One_And_Zero) {

    struct BinArray* arrs = nullptr;

    const float koef = 0.02f;

    const size_t num_arr = koef * num_arr_g;   

    // --------------------------------------------------

    arrs = GetArrayBA (num_arr);
    RandRetainArrayBA (arrs, num_arr, 1, max_size_arr_g);

    FillOneArrayBA (arrs, num_arr);
    CheckOneFromGetValueArrayBA (arrs, num_arr);

    ReleaseArrayBA (arrs, num_arr);

    // --------------------------------------------------

    arrs = GetArrayBA (num_arr);
    RandRetainArrayBA (arrs, num_arr, 1, max_size_arr_g);

    FillZeroArrayBA (arrs, num_arr);
    CheckZeroFromGetValueArrayBA (arrs, num_arr);

    ReleaseArrayBA (arrs, num_arr);

    // --------------------------------------------------

    delete [] arrs;
}

TEST (BIN_ARRAY, Setting_And_Getting_One_Or_Zero) {

    struct BinArray* arrs = nullptr;

    const float koef = 0.005f;

    const size_t num_arr = koef * num_arr_g;   

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