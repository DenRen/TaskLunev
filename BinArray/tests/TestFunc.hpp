#ifndef TESTFUNC_HPP
#define TESTFUNC_HPP

#include "../BinArray.h"
#include <cstddef>

struct BinArray* GetArrayBA (size_t num);

void RandRetainArrayBA (struct BinArray arrs[], size_t num, 
                        size_t size_from, size_t size_to);

void RandResizeArrayBA (struct BinArray arrs[], size_t num, 
                        size_t size_from, size_t size_to);

void ReleaseArrayBA (struct BinArray arrs[], size_t num);

void FillOneArrayBA  (struct BinArray arrs[], size_t num);
void FillZeroArrayBA (struct BinArray arrs[], size_t num);

void CheckOneArrayBA  (struct BinArray arrs[], size_t num) ;
void CheckZeroArrayBA (struct BinArray arrs[], size_t num);

void CheckOneFromGetValueArrayBA  (struct BinArray arrs[], size_t num);
void CheckZeroFromGetValueArrayBA (struct BinArray arrs[], size_t num);

void CheckSetAndGetOneArrayBA  (struct BinArray arrs[], size_t num);
void CheckSetAndGetZeroArrayBA (struct BinArray arrs[], size_t num);

#endif /* TESTFUNC_HPP */