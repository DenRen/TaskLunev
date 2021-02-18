#ifndef TESTFUNC_HPP
#define TESTFUNC_HPP

#include <cstddef>
#include "../BinArray.h"

BinArray** GetArrayBA (size_t num);

void RandRetainArrayBA (BinArray* arrs[], size_t num, 
                        size_t size_from, size_t size_to);

void RandResizeArrayBA (BinArray* arrs[], size_t num, 
                        size_t size_from, size_t size_to);

void ReleaseArrayBA (BinArray* arrs[], size_t num);

void FillOneArrayBA  (BinArray* arrs[], size_t num);
void FillZeroArrayBA (BinArray* arrs[], size_t num);
void FillRandArrayBA (BinArray* arrs[], size_t num);

void CheckOneArrayBA  (BinArray* arrs[], size_t num) ;
void CheckZeroArrayBA (BinArray* arrs[], size_t num);

void CheckSetAndGetOneArrayBA  (BinArray* arrs[], size_t num);
void CheckSetAndGetZeroArrayBA (BinArray* arrs[], size_t num);

void CheckFindOne  (BinArray* arrs[], size_t num, const float koef_num_check);
void CheckFindZero (BinArray* arrs[], size_t num, const float koef_num_check);
void CheckFind (BinArray* arrs[], size_t num, const float koef_num_check);

#endif /* TESTFUNC_HPP */