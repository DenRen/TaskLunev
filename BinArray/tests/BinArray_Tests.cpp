#include <gtest/gtest.h>
#include <random>

#include "../BinArray.h"
#include "TestFunc.hpp"

const size_t num_arr_g = 5000                / 1;
const size_t max_size_arr_g = UINT16_MAX * 4 / 1;

// ===================\\
// Secondary functions --------------------------------------------------------
// ===================//

#ifdef PROBABLE_FAULT_ALLOCATORS
TEST (BIN_ARRAY, Fault_Allocate) {
    ASSERT_TRUE (baCreate (KILL_ALLOCATOR) == NULL);

    BinArray* arr = baCreate (2);
    ASSERT_TRUE (arr != NULL);

    ASSERT_TRUE (baResize (arr, KILL_ALLOCATOR) == -1);
    baDestroy (&arr);
}
#endif
TEST (BIN_ARRAY, Secondary_Functions) {
    ASSERT_EQ (baBits2Bytes (0),  0);
    ASSERT_EQ (baBits2Bytes (2),  1);
    ASSERT_EQ (baBits2Bytes (5),  1);
    ASSERT_EQ (baBits2Bytes (8),  1);
    ASSERT_EQ (baBits2Bytes (15), 2);
    ASSERT_EQ (baBits2Bytes (16), 2);
    ASSERT_EQ (baBits2Bytes (17), 3);
    ASSERT_EQ (baBits2Bytes (8 * 1024 * 1024 + 3), 1024 * 1024 + 1);

    // Testing stdout through intercepting a stream

    #define TEST_PRINT_BYTE(byte)    ASSERT_EQ (Check_print_byte    (0b ## byte, #byte),     true);
    #define TEST_PRINT_BYTE_NL(byte) ASSERT_EQ (Check_print_byte_nl (0b ## byte, #byte"\n"), true);
    #define TEST_PRINTS(byte) TEST_PRINT_BYTE (byte); TEST_PRINT_BYTE_NL (byte);

    TEST_PRINTS (00000000);
    TEST_PRINTS (11111111);

    TEST_PRINTS (10000000);
    TEST_PRINTS (01000000);
    TEST_PRINTS (00100000);
    TEST_PRINTS (00010000);
    TEST_PRINTS (00001000);
    TEST_PRINTS (00000100);
    TEST_PRINTS (00000010);
    TEST_PRINTS (00000001);

    TEST_PRINTS (00000001);
    TEST_PRINTS (00110101);
    TEST_PRINTS (00110001);
    TEST_PRINTS (00000001);
    TEST_PRINTS (00001101);
    TEST_PRINTS (11100101);
    TEST_PRINTS (01100101);

    #undef TEST_PRINTS
    #undef TEST_PRINT_BYTE
    #undef TEST_PRINT_BYTE_NL

    uint8_t byte = 0;
    uint8_t bytes[2];

    #define TEST_BPRINT8(val_byte)                              \
        byte = 0b ## val_byte;                                  \
        ASSERT_EQ (Check_bprint (&byte, 1, #val_byte" "), true);

    #define TEST_BPRINT16(val_byte0, val_byte1)                                    \
        bytes[0] = 0b ## val_byte0;                                                \
        bytes[1] = 0b ## val_byte1;                                                \
        ASSERT_EQ (Check_bprint (bytes, 2, #val_byte0" "#val_byte1" "), true);
    
    TEST_BPRINT8 (00000000);
    TEST_BPRINT8 (11111111);
    TEST_BPRINT8 (00001111);
    TEST_BPRINT8 (11110000);
    
    TEST_BPRINT8 (00001000);
    TEST_BPRINT8 (00100010);
    TEST_BPRINT8 (01001000);
    TEST_BPRINT8 (01101011);
    TEST_BPRINT8 (00001100);
    TEST_BPRINT8 (10110011);

    TEST_BPRINT16 (00000000, 00000000);
    TEST_BPRINT16 (00000000, 11111111);
    TEST_BPRINT16 (11111111, 00000000);
    TEST_BPRINT16 (11111111, 11111111);

    TEST_BPRINT16 (00000000, 00000000);
    TEST_BPRINT16 (00000000, 00001111);
    TEST_BPRINT16 (00000000, 11110000);
    TEST_BPRINT16 (00000000, 11111111);
    
    TEST_BPRINT16 (00000000, 00000000);
    TEST_BPRINT16 (00001111, 00000000);
    TEST_BPRINT16 (11110000, 00000000);
    TEST_BPRINT16 (11111111, 00000000);

    TEST_BPRINT16 (00001000, 00001001);
    TEST_BPRINT16 (00100110, 01010011);
    TEST_BPRINT16 (00001101, 00101001);
    TEST_BPRINT16 (00100011, 01010011);
    TEST_BPRINT16 (01001000, 11101001);
    TEST_BPRINT16 (01100010, 01110011);

    #undef TEST_BPRINT16
    #undef TEST_BPRINT8
}

TEST (BIN_ARRAY, GetNumBits) {

    const size_t num_bits = 1456465;

    BinArray* arr = baCreate (num_bits);
    ASSERT_TRUE (arr != NULL);

    ASSERT_EQ (baGetNumBits (arr), num_bits);
    baDestroy (&arr);
}

// ===============\\
// Check functions ------------------------------------------------------------
// ===============//

TEST (BIN_ARRAY, baCheckCalcArg) {
    BinArray* arr = baCreate (2);
    ASSERT_TRUE (arr != NULL);

    ASSERT_TRUE (baGetSubArray (NULL, 4, -2) == NULL);
    ASSERT_TRUE (baGetSubArray (arr,  4, -2) == NULL);
    ASSERT_TRUE (baGetSubArray (arr,  2,  3) == NULL);
    ASSERT_TRUE (baGetSubArray (arr,  0,  3) == NULL);

    baDestroy (&arr);
}

// ==================\\
// Creation functions ---------------------------------------------------------
// ==================//

TEST (BIN_ARRAY, Create_Resize_Release) { 
    ASSERT_TRUE (baCreate (0) == NULL);
    ASSERT_TRUE (baCreate (0) == NULL);

    BinArray* arr = baCreate (1);
    ASSERT_TRUE (arr != NULL);
    ASSERT_TRUE (baResize (arr,  0) == -1);
    ASSERT_TRUE (baResize (NULL, 0) == -1);
    baDestroy (&arr);

    ASSERT_TRUE (baDestroy (NULL) == -1);
}

TEST (BIN_ARRAY, Rand_Create_Resize_Release) {
    
    const float koef_num_arr = 50;

    const size_t num_arr = koef_num_arr * num_arr_g;   
    
    BinArray** arrs = nullptr;
    // --------------------------------------------------

    arrs = GetArrayBA (num_arr);
    RandCreateArrayBA (arrs, num_arr, 1, max_size_arr_g);
    DestroyArrayBA (arrs, num_arr);

    // --------------------------------------------------

    arrs = GetArrayBA (num_arr);
    RandCreateArrayBA (arrs, num_arr, 1, max_size_arr_g);
    RandResizeArrayBA (arrs, num_arr, 1, max_size_arr_g);
    DestroyArrayBA (arrs, num_arr);

    // --------------------------------------------------

    delete [] arrs;
}

TEST (BIN_ARRAY, GetCLone) {
    ASSERT_TRUE (baGetClone (NULL) == NULL);
}

TEST (BIN_ARRAY, Rand_GetCLone) {
    const float koef_num_arr = 0.02;

    const size_t num_arr = koef_num_arr * num_arr_g;

    BinArray** arrs = nullptr;    
    // --------------------------------------------------

    arrs = GetArrayBA (num_arr);
    RandCreateArrayBA (arrs, num_arr, 1, max_size_arr_g);

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

TEST (BIN_ARRAY, GetSubArray) {
    ASSERT_TRUE (baGetSubArray (NULL, 4, 3) == NULL);

    BinArray* arr = baCreate (8);
    ASSERT_TRUE (arr != NULL);

    BinArray* sub_arr = baGetSubArray (arr, 0, 3);
    ASSERT_TRUE (sub_arr != NULL);
    baDestroy (&sub_arr);

    sub_arr = baGetSubArray (arr, 1, 3);
    ASSERT_TRUE (sub_arr != NULL);
    baDestroy (&sub_arr);

    ASSERT_TRUE (baResize (arr, 100) != -1);

    sub_arr = baGetSubArray (arr, 10, 80);
    ASSERT_TRUE (sub_arr != NULL);
    baDestroy (&sub_arr);

    // Across fill 

    baFillOneFull (arr);
    sub_arr = baGetSubArray (arr, 10, 80);
    ASSERT_TRUE (sub_arr != NULL);

    for (int i = 0; i < 80; ++i) {
        ASSERT_EQ (baGetValue (sub_arr, i), 1) << i
                                               << "AAA\n" << baDumpBufFull (sub_arr)
                                               << " " << baGetNumBits (sub_arr);
    }
    baDestroy (&sub_arr);

    baFillZeroFull (arr);
    sub_arr = baGetSubArray (arr, 10, 80);
    ASSERT_TRUE (sub_arr != NULL);

    for (int i = 0; i < 80; ++i) {
        ASSERT_EQ (baGetValue (sub_arr, i), 0) << i 
                                               << "\n" << baDumpBufFull (sub_arr);
    }
    baDestroy (&sub_arr);

    baDestroy (&arr);
}

// =============================\\
// Getters and setters functions ----------------------------------------------
// Fill functions //============//
// ==============//

TEST (BIN_ARRAY, Filling_And_Getting_One_And_Zero) {
    ASSERT_EQ (baFillOneFull  (NULL), -1);
    ASSERT_EQ (baFillZeroFull (NULL), -1);

    ASSERT_EQ (baFillOne  (NULL, 1, 1), -1);
    ASSERT_EQ (baFillZero (NULL, 1, 1), -1);

    // ---------------------------------------

    BinArray* arr = baCreate (5);
    ASSERT_TRUE (arr != NULL);

    ASSERT_EQ (baSetValue (arr, 6, 1), -1);
    ASSERT_EQ (baSetValue (arr, 6, 0), -1);
    ASSERT_EQ (baSetOne   (arr, 6), -1);
    ASSERT_EQ (baSetZero  (arr, 6), -1);

    ASSERT_EQ (baGetValue (arr, 6), -1);

    baDestroy (&arr);

    // ---------------------------------------

    CheckFillOneBA  (  8, 3,   4);
    CheckFillOneBA  (  6, 0,   2);
    CheckFillOneBA  (800, 3,  -1);
    CheckFillOneBA  (800, 3, 145);
    CheckFillOneBA  (78,  3,  -1);

    CheckFillZeroBA (  8, 3,   4);
    CheckFillZeroBA (  6, 0,   2);
    CheckFillZeroBA (800, 3,  -1);
    CheckFillZeroBA (800, 3, 145);
    CheckFillZeroBA (78,  3,  -1);

    for (int i = 4; i < 200; ++i)
        CheckFillZeroBA (i, 3, -1);
}

TEST (BIN_ARRAY, Rand_Filling_And_Getting_One_And_Zero) {

    const float koef_num_arr = 0.05f;

    const size_t num_arr = koef_num_arr * num_arr_g;

    BinArray** arrs = nullptr;

    // --------------------------------------------------

    arrs = GetArrayBA (num_arr);
    RandCreateArrayBA (arrs, num_arr, 1, max_size_arr_g);

    FillOneFullArrayBA (arrs, num_arr);
    CheckOneFullArrayBA (arrs, num_arr);

    DestroyArrayBA (arrs, num_arr);

    // --------------------------------------------------

    arrs = GetArrayBA (num_arr);
    RandCreateArrayBA (arrs, num_arr, 1, max_size_arr_g);

    FillZeroFullArrayBA (arrs, num_arr);
    CheckZeroFullArrayBA (arrs, num_arr);

    DestroyArrayBA (arrs, num_arr);

    // --------------------------------------------------

    delete [] arrs;
}

TEST (BIN_ARRAY, Setting_And_GettingOne_OrZero) {

    const float koef_num_arr = 0.005f;

    const size_t num_arr = koef_num_arr * num_arr_g;   

    BinArray** arrs = nullptr;

    // --------------------------------------------------

    arrs = GetArrayBA (num_arr);
    RandCreateArrayBA (arrs, num_arr, 1, max_size_arr_g);

    CheckSetAndGetOneArrayBA (arrs, num_arr);

    DestroyArrayBA (arrs, num_arr);

    // --------------------------------------------------

    arrs = GetArrayBA (num_arr);
    RandCreateArrayBA (arrs, num_arr, 1, max_size_arr_g);

    CheckSetAndGetZeroArrayBA (arrs, num_arr);

    DestroyArrayBA (arrs, num_arr);

    // --------------------------------------------------

    delete [] arrs;
}

// ==============\\
// Find functions -------------------------------------------------------------
// ==============//

TEST (BIN_ARRAY, Find) {

    ASSERT_EQ (baFindOne  (NULL, 0, 3), -1);
    ASSERT_EQ (baFindZero (NULL, 0, 3), -1);

    // ----------------------------------------

    BinArray* arr = baCreate (10);
    ASSERT_TRUE (arr != NULL);

    baFillZeroFull (arr);

    baSetOne (arr, 4);
    baSetOne (arr, 5);
    ASSERT_EQ (baFindOne (arr, 0, 6), 4);
    ASSERT_EQ (baFindOne (arr, 3, 7), 4);
    
    baInvert (arr, 0, -1);
    ASSERT_EQ (baFindZero (arr, 0, 5), 4);
    ASSERT_EQ (baFindZero (arr, 3, 7), 4);

    baDestroy (&arr);

    // ----------------------------------------

    arr = baCreate (100);
    ASSERT_TRUE (arr != NULL);

    baFillZeroFull (arr);

    baSetOne (arr, 70);
    ASSERT_EQ (baFindOne (arr, 3, -1), 70);
    baSetOne (arr, 53);
    ASSERT_EQ (baFindOne (arr, 3, -1), 53);
      
    baFillOneFull (arr);

    baSetZero (arr, 70);
    ASSERT_EQ (baFindZero (arr, 3, -1), 70);
    baSetZero (arr, 53);
    ASSERT_EQ (baFindZero (arr, 3, -1), 53);

    baDestroy (&arr);
}

TEST (BIN_ARRAY, FindFull) {
    const float koef_num_arr = 0.01;
    const float koef_num_check = 0.01;

    const size_t num_arr = koef_num_arr * num_arr_g;
    
    BinArray** arrs = nullptr;


    // Little arrays ------------------------------------

    arrs = GetArrayBA (num_arr);
    RandCreateArrayBA (arrs, num_arr, 1, 8);

    // --------------------------------------------------

    CheckFindOne  (arrs, num_arr, koef_num_check);
    CheckFindZero (arrs, num_arr, koef_num_check);
    CheckFind     (arrs, num_arr, koef_num_check);

    DestroyArrayBA (arrs, num_arr);

    // Normal arrays ------------------------------------
    
    arrs = GetArrayBA (num_arr);
    RandCreateArrayBA (arrs, num_arr, 1, max_size_arr_g);

    // --------------------------------------------------

    CheckFindOne  (arrs, num_arr, koef_num_check);
    CheckFindZero (arrs, num_arr, koef_num_check);
    CheckFind     (arrs, num_arr, koef_num_check);
    
    DestroyArrayBA (arrs, num_arr);

    // --------------------------------------------------

    delete [] arrs;
}

// ================\\
// Foreach function -----------------------------------------------------------
// ================//

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

TEST (BIN_ARRAY, Foreach_On_Max_Min_Functions) {

    const float koef_num_arr = 0.1;

    const size_t num_arr = koef_num_arr * num_arr_g;

    BinArray** arrs = nullptr;

    // --------------------------------------------------

    arrs = GetArrayBA (num_arr);
    RandCreateArrayBA (arrs, num_arr, 1, max_size_arr_g);

    // --------------------------------------------------

    for (int i = 0; i < num_arr; ++i) {
        bool max = false;
        baForeach (arrs[i], find_max, &max);

        if (baFindOne (arrs[i], 0, -1) == -1) {
            ASSERT_FALSE (max);
        } else {
            ASSERT_TRUE (max);
        }
    }

    // --------------------------------------------------

    for (int i = 0; i < num_arr; ++i) {
        bool min = true;
        baForeach (arrs[i], find_min, &min);

        if (baFindZero (arrs[i], 0, -1) == -1) {
            ASSERT_TRUE (min);
        } else {
            ASSERT_FALSE (min);
        }
    }

    // --------------------------------------------------

    DestroyArrayBA (arrs, num_arr);

    delete [] arrs;
}

// ================\\
// Invert functions -----------------------------------------------------------
// ================//

TEST (BIN_ARRAY, Invert_And_GetInvert) {
    ASSERT_TRUE (baInvert    (NULL, 0, -1) == -1);
    ASSERT_TRUE (baGetInvert (NULL) == NULL);    
}

TEST (BIN_ARRAY, Rand_Invert_And_GetInvert) {
    const float koef_num_arr = 0.01;

    const size_t num_arr = koef_num_arr * num_arr_g;

    BinArray** arrs = nullptr;
    
    // --------------------------------------------------
    
    arrs = GetArrayBA (num_arr);
    RandCreateArrayBA (arrs, num_arr, 1, max_size_arr_g);

    // Invert -------------------------------------------

    FillRandArrayBA (arrs, num_arr);
    for (size_t iarr = 0; iarr < num_arr; ++iarr) {
        BinArray* arr = arrs[iarr];
        
        BinArray* inv_arr = baGetClone (arr);
        baInvert (inv_arr, 0, -1);

        size_t num_bits = baGetNumBits (arr);
        for (int i = 0; i < num_bits; i++) {
            bool elem = baGetValue (arr, i);
            bool inv_elem = baGetValue (inv_arr, i);

            ASSERT_TRUE (elem != inv_elem) << baDumpBufFull (arr) << "\n\n" 
                                           << baDumpBufFull (inv_arr);
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

// ==============\\
// Dump functions -------------------------------------------------------------
// ==============//

TEST (BIN_ARRAY, Dump) {

    BinArray* arr = baCreate (5);
    ASSERT_TRUE (arr != NULL);

    baFillZeroFull (arr);
    ASSERT_EQ (Check_Dump (arr, 6, -1, "00000"), 0);
    ASSERT_EQ (Check_Dump (arr, 0, -1, "00000"), 1);
    ASSERT_EQ (Check_Dump (arr, 0,  2, "00"   ), 1);
    ASSERT_EQ (Check_Dump (arr, 1,  2, "00"   ), 1);
    ASSERT_EQ (Check_Dump (arr, 3, -1, "00"   ), 1);
    ASSERT_EQ (Check_Dump (arr, 0,  1, "0"    ), 1);
    ASSERT_EQ (Check_Dump (arr, 3,  2, "00"   ), 1);

    baFillOneFull (arr);
    ASSERT_EQ (Check_Dump (arr, 6, -1, "11111"), 0);
    ASSERT_EQ (Check_Dump (arr, 0, -1, "11111"), 1);
    ASSERT_EQ (Check_Dump (arr, 0,  2, "11"   ), 1);
    ASSERT_EQ (Check_Dump (arr, 1,  2, "11"   ), 1);
    ASSERT_EQ (Check_Dump (arr, 3, -1, "11"   ), 1);
    ASSERT_EQ (Check_Dump (arr, 0,  1, "1"    ), 1);
    ASSERT_EQ (Check_Dump (arr, 3, 2,  "11"   ), 1);

    baResize (arr, 16);
    ASSERT_TRUE (arr != NULL);

    baFillZeroFull (arr);
    ASSERT_EQ (Check_Dump (arr, 6, -1, "00000000 00000000"), 0);
    ASSERT_EQ (Check_Dump (arr, 6, -1, "00000000 00011100"), 0);
    ASSERT_EQ (Check_Dump (arr, 6, -1, "11111111 11111111"), 0);

    baDestroy (&arr);
}