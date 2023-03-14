#include <stdio.h>

#include "abacus.h"

int main() {
    char *str;
    char *numOne = "1000";
    char *numTwo = "9000";
    sAbacus abaOne, abaTwo, res;

    abacus_set(&abaOne, numOne);
    abacus_set(&abaTwo, numTwo);

    abacus_print(abaOne);
    printf("%s\n", abacus_getNumber(abaOne));
    abacus_print(abaTwo);
    printf("%s\n", abacus_getNumber(abaTwo));

    abacus_add(&res, abaOne, abaTwo);

    printf("Hello World\n");

    abacus_print(res);
    printf("%s\n", abacus_getNumber(res));
    return 0;
}
