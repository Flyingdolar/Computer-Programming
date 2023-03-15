#include <stdio.h>

#include "abacus.h"

int main() {
    char *str;
    char *numOne = "55555555";
    char *numTwo = "4444a445";
    int rec = 0;

    sAbacus abaOne, abaTwo, res;

    rec = abacus_set(&abaOne, numOne);
    printf("rec = %d\n", rec);
    rec = abacus_set(&abaTwo, numTwo);
    printf("rec = %d\n", rec);

    rec = abacus_print(abaOne);
    printf("rec = %d\n", rec);
    printf("%s\n", abacus_getNumber(abaOne));
    rec = abacus_print(abaTwo);
    printf("rec = %d\n", rec);
    printf("%s\n", abacus_getNumber(abaTwo));

    rec = abacus_add(&res, abaOne, abaTwo);
    printf("rec = %d\n", rec);
    printf("Hello World\n");

    rec = abacus_print(res);
    printf("rec = %d\n", rec);
    printf("%s\n", abacus_getNumber(res));
    return 0;
}
