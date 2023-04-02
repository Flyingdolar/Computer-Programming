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

sMatrix *matrix_init(uint8_t row, uint8_t col) {
    if (row <= 0 || col <= 0) return NULL;
    pMatrix newMtx = (pMatrix)malloc(sizeof(sMatrix));
    newMtx->rows = row, newMtx->cols = col;

    newMtx->data = (int32_t **)malloc(row * sizeof(int32_t *));
    for (uint32_t idx = 0; idx < row; idx++) {
        newMtx->data[idx] = (int32_t *)malloc(col * sizeof(int32_t));
        for (uint32_t jdx = 0; jdx < col; jdx++)
            newMtx->data[idx][jdx] = 0;
    };
    return newMtx;
}

int32_t matrix_set(sMatrix *pMtx, uint8_t row, uint8_t col, int32_t value) {
    if (row < 0 || col < 0) return -1;
    if (row >= pMtx->rows || col >= pMtx->cols) return -1;

    pMtx->data[row][col] = value;
    return 0;
}

void matrix_print(const sMatrix *pMtx) {
    for (uint32_t idx = 0; idx < pMtx->rows; idx++) {
        for (uint32_t jdx = 0; jdx < pMtx->cols; jdx++)
            printf("%d ", pMtx->data[idx][jdx]);
        printf("\n");
    }
    return;
}

int32_t matrix_add(sMatrix *pMtxA, const sMatrix *pMtxB, const sMatrix *pMtxC) {
    if (pMtxA == NULL || pMtxB == NULL || pMtxC == NULL) return -1;
    if (pMtxB->rows != pMtxC->rows || pMtxB->cols != pMtxC->cols) return -1;
    if (pMtxA->rows != pMtxC->rows || pMtxA->cols != pMtxC->cols) return -1;

    for (uint8_t idx = 0; idx < pMtxA->rows; idx++)
        for (uint8_t jdx = 0; jdx < pMtxA->cols; jdx++)
            pMtxA->data[idx][jdx] = pMtxB->data[idx][jdx] + pMtxC->data[idx][jdx];
    return 0;
}

int32_t matrix_multiply(sMatrix *pMtxA, const sMatrix *pMtxB, const sMatrix *pMtxC) {
    if (pMtxA == NULL || pMtxB == NULL || pMtxC == NULL) return -1;
    if (pMtxB->cols != pMtxC->rows) return -1;
    if (pMtxA->rows != pMtxB->rows || pMtxA->cols != pMtxC->cols) return -1;

    for (uint8_t idx = 0; idx < pMtxA->rows; idx++) {
        for (uint8_t jdx = 0; jdx < pMtxA->cols; jdx++) {
            int32_t sum = 0;
            for (uint8_t kdx = 0; kdx < pMtxB->cols; kdx++)
                sum += pMtxB->data[idx][kdx] * pMtxC->data[kdx][jdx];
            pMtxA->data[idx][jdx] = sum;
        }
    }
    return 0;
}

int32_t matrix_transpose(sMatrix *pMtx) {
    if (pMtx == NULL) return -1;
    pMatrix temp = matrix_init(pMtx->cols, pMtx->rows);

    for (uint8_t idx = 0; idx < pMtx->rows; idx++) {
        for (uint8_t jdx = 0; jdx < pMtx->cols; jdx++)
            temp->data[jdx][idx] = pMtx->data[idx][jdx];
    }
    for (uint8_t idx = 0; idx < pMtx->rows; idx++) free(pMtx->data[idx]);
    free(pMtx->data);
    pMtx->rows = temp->rows, pMtx->cols = temp->cols;
    pMtx->data = temp->data;
    return 0;
}

int32_t matrix_det(const sMatrix *pMtx, int32_t *pAns) {
    if (pMtx == NULL || pAns == NULL) return -1;
    if (pMtx->rows != pMtx->cols) return -1;

    uint32_t len = pMtx->rows;
    double det = 1, *tmpRow, **ppCopy = (double **)malloc(len * sizeof(double *));
    for (uint32_t idx = 0; idx < len; idx++) {
        ppCopy[idx] = (double *)malloc(len * sizeof(double));
        for (uint32_t jdx = 0; jdx < len; jdx++)
            ppCopy[idx][jdx] = pMtx->data[idx][jdx];
    }

    for (uint32_t idx = 0, kdx = idx; idx < len; idx++, kdx = idx) {
        while (kdx < len && ppCopy[kdx][idx] == 0) kdx++;
        if (kdx == len) {
            det = 0;
            break;
        }
        if (kdx != idx) {
            det = -det, tmpRow = ppCopy[idx];
            ppCopy[idx] = ppCopy[kdx], ppCopy[kdx] = tmpRow;
        }
        det *= ppCopy[idx][idx];
        for (uint32_t idxN = idx + 1; idxN < len; idxN++) {
            double scale = ppCopy[idxN][idx] / ppCopy[idx][idx];
            for (uint32_t jdx = idx; jdx < len; jdx++)
                ppCopy[idxN][jdx] -= ppCopy[idx][jdx] * scale;
        }
    }
    for (uint32_t idx = 0; idx < len; idx++) free(ppCopy[idx]);
    free(ppCopy), *pAns = round(det);
    return 0;
}

int32_t matrix_free(sMatrix *pMtx) {
    if (pMtx == NULL) return -1;

    for (uint32_t idx = 0; idx < pMtx->rows; idx++)
        free(pMtx->data[idx]);
    free(pMtx->data);
    free(pMtx), pMtx = NULL;
    return 0;
};
