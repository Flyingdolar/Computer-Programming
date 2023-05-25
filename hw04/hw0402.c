#include <stdint.h>
#include <stdio.h>

#include "macros.h"

// /* Bits related Macros */
// // Return 2 to x. You do not need to worry the overflow issue.
// #define BIT(x) 1 << x
// // Set x's p-th bit to 1. Count from 0 and from LSB.
// #define SETBIT(x, p) x | BIT(p)
// // Set x's p-th bit to 0. Count from 0 and from LSB.
// #define CLEARBIT(x, p) x & ~BIT(p)
// // Get x's p-th bit to 1. Count from 0 and from LSB.
// #define GETBIT(x, p) (x >> p) & 1
// // Toggle x's p-th bit to 1. Count from 0 and from LSB.
// #define TOGGLEBIT(x, p) (x >> p) & 1 ? CLEARBIT(x, p) : SETBIT(x, p)

// /* Loop related Macros */
// // Example: RANGE (i, 10, 20)
// // it will be 10 ,11 ,12 ,13 ,14 ,15,16 ,17 ,18 ,19 ,20
// // Example: RANGE (i, 5, -5)
// // it will be 5 ,4 ,3 ,2 ,1 ,0 ,-1 ,-2 ,-3 ,-4 ,-5
// #define RANGE(item, start, end) for (item = start; item <= end; item++)
// // FOREACH (item , array)
// // A C implementation of foreach loop , it will go through each element of an
// // array , and will perform operations on each element as returned into
// // pointer item variable.
// #define FOREACH(item, Array)                                                 \
//     for (idx = 0, item = Array[idx]; idx < sizeof(Array) / sizeof(Array[0]); \
//          idx++, item = Array[idx])

int main(int argc, char *argv[]) {
    printf("MACROS: BIT\n");
    printf("BIT(0) = %d\n", BIT(0));
    printf("BIT(1) = %d\n", BIT(1));
    printf("BIT(2) = %d\n", BIT(2));
    printf("MACROS: SETBIT\n");
    printf("SETBIT(0, 0) = %d\n", SETBIT(0, 0));
    printf("SETBIT(0, 1) = %d\n", SETBIT(0, 1));
    printf("SETBIT(0, 2) = %d\n", SETBIT(0, 2));
    printf("SETBIT(1, 0) = %d\n", SETBIT(1, 0));
    printf("SETBIT(2, 1) = %d\n", SETBIT(2, 1));
    printf("SETBIT(4, 2) = %d\n", SETBIT(4, 2));

    return 0;
}
