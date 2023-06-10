#include <getopt.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "bmp.h"
#include "color.h"
#define PRINTE(...) printf("\033[0;31mE: "), printf(__VA_ARGS__), printf("\033[0m\n")

typedef enum _optCmd_ {
    INPUT,
    HELP
} optCmd;

struct option long_options[] = {
    {"input", 1, NULL, 'i'},
    {"help", 0, NULL, 'h'},
    {NULL, 0, NULL, 0}};

void printHelp() {
    PRINTC(C_YELLOW, A_DEFAULT, "Using Help Command...\n");
    printf("+==============================+\n");
    printf("| fin01 - Title |\n");
    printf("+==============================+\n");
    printf("some descriptions...\n");

    printf("\n* Usages *\n");
    printf("cmd [Options]...\n");
    printf("\n* Options *\n");
    printf(" %s / %-10s\t | %-15s\t | %-20s\n", "Short", "Long Opt.", "Variable (Default)", "Function");
    printf("-------------------------+-----------------------+---------------------------------\n");
    printf(" %3s    %-10s\t | %-15s\t | %-20s\n", "-i", "--input", "[file]", "Input file name");
    return;
}

int main(int argc, char *argv[]) {
    bool cmd[1] = {false};
    int opt;
    char *input = NULL;

    while ((opt = getopt_long(argc, argv, "i:h", long_options, NULL)) != -1) {
        if (opt == 'h') printHelp(), cmd[HELP] = true;
        if (opt == 'i') input = optarg, cmd[INPUT] = true;
        if (opt == '?') {
            PRINTE("Invalid option %s", argv[optind - 1]);
            return -1;
        }
    }

    return 0;
}
