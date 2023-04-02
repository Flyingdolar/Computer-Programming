#include <stdio.h>
#include <string.h>

#include "matrix.h"

int main() {
    pMatrix M_numA = matrix_init(4, 4);
    pMatrix M_numB = matrix_init(4, 4);
    pMatrix M_Add = matrix_init(4, 4);

    printf("Set to Zero:\n");
    matrix_print(M_numA);

    if (matrix_set(M_numA, 2, 2, 10) == -1) printf("Set Error\n");
    if (matrix_set(M_numB, 1, 0, 5) == -1) printf("Set Error\n");
    if (matrix_add(M_Add, M_numA, M_numB) == -1) printf("Add Error\n");

    printf("Add Result:\n");
    matrix_print(M_Add);

    pMatrix M_numC = matrix_init(5, 3);
    pMatrix M_numD = matrix_init(3, 2);
    pMatrix M_Multi = matrix_init(5, 2);

    if (matrix_set(M_numC, 0, 2, 5) == -1) printf("Set Error\n");
    if (matrix_set(M_numC, 2, 2, 10) == -1) printf("Set Error\n");
    if (matrix_set(M_numC, 1, 3, 7) == -1) printf("Set Error\n");  // This would error.
    if (matrix_set(M_numC, 4, 0, -10) == -1) printf("Set Error\n");
    if (matrix_set(M_numC, 3, 1, -3) == -1) printf("Set Error\n");

    if (matrix_set(M_numD, 1, 0, 5) == -1) printf("Set Error\n");
    if (matrix_set(M_numD, 0, 0, 5) == -1) printf("Set Error\n");
    if (matrix_set(M_numD, 2, 0, -2) == -1) printf("Set Error\n");
    if (matrix_set(M_numD, 0, 1, -6) == -1) printf("Set Error\n");
    if (matrix_set(M_numD, 0, 0, 3) == -1) printf("Set Error\n");

    printf("Matrix C & D:\n");
    matrix_print(M_numC), matrix_print(M_numD);

    if (matrix_multiply(M_Multi, M_numC, M_numD) == -1) printf("Multiply Error\n");

    printf("Multiply Result:\n");
    matrix_print(M_Multi);

    if (matrix_transpose(M_Multi) == -1) printf("Transpose Error\n");

    printf("Transpose Result:\n");
    matrix_print(M_Multi);

    matrix_set(M_Add, 0, 0, 20);
    matrix_set(M_Add, 0, 1, -100);
    matrix_set(M_Add, 0, 2, 3999);
    matrix_set(M_Add, 0, 3, 328);
    matrix_set(M_Add, 1, 0, -48);
    matrix_set(M_Add, 1, 1, 0);
    matrix_set(M_Add, 1, 2, 28);
    matrix_set(M_Add, 1, 3, 38);
    matrix_set(M_Add, 2, 0, 40);
    matrix_set(M_Add, 2, 1, -100);
    matrix_set(M_Add, 2, 2, 3999);
    matrix_set(M_Add, 2, 3, 328);
    matrix_set(M_Add, 3, 0, 5);
    matrix_set(M_Add, 3, 1, -5);
    matrix_set(M_Add, 3, 2, 4);
    matrix_set(M_Add, 3, 3, 9);

    printf("Matrix for Determine:\n");
    matrix_print(M_Add);

    int32_t num = 0;
    matrix_det(M_Add, &num);
    printf("Determine = %d\n", num);

    matrix_free(M_numA), matrix_free(M_numB), matrix_free(M_numC), matrix_free(M_numD);
    matrix_free(M_Add), matrix_free(M_Multi);
    return 0;
}
