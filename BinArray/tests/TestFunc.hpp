#ifndef TESTFUNC_HPP
#define TESTFUNC_HPP

#include <cstddef>
#include "../BinArray.h"

void ReadFromStdoutToBuf (char* buf, uint8_t size);

BinArray** GetArrayBA (size_t num);

void RandCreateArrayBA (BinArray* arrs[], size_t num, 
                        size_t size_from, size_t size_to);

void RandResizeArrayBA (BinArray* arrs[], size_t num, 
                        size_t size_from, size_t size_to);

void DestroyArrayBA (BinArray* arrs[], size_t num);

void FillOneFullArrayBA  (BinArray* arrs[], size_t num);
void FillZeroFullArrayBA (BinArray* arrs[], size_t num);
void FillRandArrayBA (BinArray* arrs[], size_t num);

void CheckFillOneBA  (size_t num_bits, size_t begin, ssize_t len);
void CheckFillZeroBA (size_t num_bits, size_t begin, ssize_t len);

void CheckOneFullBA  (BinArray* arr);
void CheckZeroFullBA (BinArray* arr);

void CheckOneFullArrayBA  (BinArray* arrs[], size_t num) ;
void CheckZeroFullArrayBA (BinArray* arrs[], size_t num);

void CheckSetAndGetOneArrayBA  (BinArray* arrs[], size_t num);
void CheckSetAndGetZeroArrayBA (BinArray* arrs[], size_t num);

void CheckFindOne  (BinArray* arrs[], size_t num, const float koef_num_check);
void CheckFindZero (BinArray* arrs[], size_t num, const float koef_num_check);
void CheckFind (BinArray* arrs[], size_t num, const float koef_num_check);

#endif /* TESTFUNC_HPP */