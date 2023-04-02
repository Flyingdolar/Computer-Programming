#pragma once
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct _sMatrix *pMatrix;
typedef struct _sMatrix {
    int32_t **data;
    uint32_t rows;
    uint32_t cols;
} sMatrix;

sMatrix *matrix_init(uint8_t row, uint8_t col);
int32_t matrix_set(sMatrix *pMtx, uint8_t row, uint8_t col, int32_t value);
void matrix_print(const sMatrix *pMtx);
int32_t matrix_add(sMatrix *pMtxA, const sMatrix *pMtxB, const sMatrix *pMtxC);
int32_t matrix_multiply(sMatrix *pMtxA, const sMatrix *pMtxB, const sMatrix *pMtxC);
int32_t matrix_transpose(sMatrix *pMtx);
int32_t matrix_det(const sMatrix *pMtx, int32_t *pAns);
int32_t matrix_free(sMatrix *pMtx);
