#include <stdint.h>
#include <stdio.h>

#include "macros.h"

int main(int argc, char *argv[]) {
    printf("BIT: %d(2^3)\n", BIT(3));
    printf("SETBIT: %d(8 set 2^4)\n", SETBIT(8, 4));
    printf("SETBIT: %d(8 set 2^3)\n", SETBIT(8, 3));
    printf("CLEARBIT: %d(24 clear 2^4)\n", CLEARBIT(24, 4));
    printf("CLEARBIT: %d(8 clear 2^3)\n", CLEARBIT(8, 3));
    printf("GETBIT: %d(8 get 2^3)\n", GETBIT(8, 3));
    printf("GETBIT: %d(8 get 2^2)\n", GETBIT(8, 2));
    printf("TOGGLEBIT: %d(8 toggle 2^3)\n", TOGGLEBIT(8, 3));
    printf("TOGGLEBIT: %d(8 toggle 2^2)\n", TOGGLEBIT(8, 2));

    int idx, start = 20, end = 10;
    printf("RANGE(%d,%d):", start, end);
    RANGE(idx, start, end) {
        printf(" %d", idx);
    }
    printf("\n");

    char *array[] = {"Hello", "World", "!"};
    char *item;
    printf("FOREACH(item, array):");
    FOREACH(item, array) {
        printf(" %s", item);
    }
    printf("\n");
    return 0;
}
