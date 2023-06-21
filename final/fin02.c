#include <getopt.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "color.h"
#define PRINTE(...) printf("\033[0;31mE: "), printf(__VA_ARGS__), printf("\033[0m\n")

typedef enum _optCmd_ {
    INPUT,
    HELP
} optCmd;

int main(int argc, char *argv[]) {
    return 0;
}
