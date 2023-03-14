#include <stdio.h>

#include "abacus.h"

int main() {
    char *num = "789748230919";
    sAbacus myAbacus;

    abacus_set(&myAbacus, num);
    abacus_print(myAbacus);
    char *str = abacus_getNumber(myAbacus);
    printf("%s\n", str);
    return 0;
}
