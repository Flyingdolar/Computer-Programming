#pragma once
#include <stdio.h>

// Set Macro for Print in Color
#define PRINTC(color, attr, ...)        \
    printf("\033[%d;%dm", color, attr); \
    printf(__VA_ARGS__);                \
    printf("\033[0m");

// Set color Number
#define C_BLACK 30
#define C_RED 31
#define C_GREEN 32
#define C_YELLOW 33
#define C_BLUE 34
#define C_MAGENTA 35
#define C_CYAN 36
#define C_WHITE 37
#define C_DEFAULT 39

// Set attr Number
#define A_BOLD 1
#define A_UNDERLINE 4
#define A_BLINK 5
#define A_REVERSE 7
#define A_HIDDEN 8
#define A_DEFAULT 0
