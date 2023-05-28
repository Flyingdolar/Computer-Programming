#pragma once
#include <math.h>
#include <stdio.h>

/* Bits related Macros */
// Return 2 to x. You do not need to worry the overflow issue.
#define BIT(x) (1 << x)
// Set x's p-th bit to 1. Count from 0 and from LSB.
#define SETBIT(x, p) (x | BIT(p))
// Set x's p-th bit to 0. Count from 0 and from LSB.
#define CLEARBIT(x, p) (x & ~BIT(p))
// Get x's p-th bit to 1. Count from 0 and from LSB.
#define GETBIT(x, p) ((x >> p) & 1)
// Toggle x's p-th bit to 1. Count from 0 and from LSB.
#define TOGGLEBIT(x, p) (GETBIT(x, p) ? CLEARBIT(x, p) : SETBIT(x, p))

/* Loop related Macros */
// Example: RANGE (i, 10, 20)
// it will be 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20
// Example: RANGE (i, 5, -5)
// it will be 5, 4, 3, 2, 1, 0, -1, -2, -3, -4, -5
#define RANGE(item, start, end)         \
    int delta = (start > end) ? -1 : 1; \
    for (item = start; item != end + delta; item += delta)
// FOREACH (item , array)
// A C implementation of foreach loop , it will go through each element of an
// array , and will perform operations on each element as returned into
// pointer item variable.
#define FOREACH(item, Array)                                                 \
    for (idx = 0, item = Array[idx]; idx < sizeof(Array) / sizeof(Array[0]); \
         idx++, item = Array[idx])
