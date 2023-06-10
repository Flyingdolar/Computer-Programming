#include <getopt.h>
#include <math.h>
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
    LEN_A,
    LAYER_N,
    OUTPUT,
    HELP
} optCmd;

struct option long_options[] = {
    {"length", 1, NULL, 'a'},
    {"layer", 1, NULL, 'n'},
    {"output", 1, NULL, 'o'},
    {"help", 0, NULL, 'h'},
    {NULL, 0, NULL, 0}};

void printHelp() {
    PRINTC(C_YELLOW, A_DEFAULT, "Using Help Command...\n");
    printf("+==============================+\n");
    printf("| fin01 - Golden Ratio |\n");
    printf("+==============================+\n");
    printf("some descriptions...\n");

    printf("\n* Usages *\n");
    printf("./fin01 -a [INT] -n [INT]\n");
    printf("\n* Options *\n");
    printf(" %s / %-10s\t | %-15s\t | %-20s\n", "Short", "Long Opt.", "Variable (Default)", "Function");
    printf("-------------------------+-----------------------+---------------------------------\n");
    printf(" %3s    %-10s\t | %-15s\t | %-20s\n", "-a", "--length", "[INT]", "Input length of the square");
    printf(" %3s    %-10s\t | %-15s\t | %-20s\n", "-n", "--layer", "[INT]", "Input layer of the square");
    printf(" %3s    %-10s\t | %-15s\t | %-20s\n", "-o", "--output", "[FILE]", "Output file name");
    return;
}

bool isInvalid(bool cmd[], int lenA, int layerN, char *output) {
    if (!cmd[LEN_A]) {
        PRINTE("Please input length of the square");
        return true;
    }
    if (!cmd[LAYER_N]) {
        PRINTE("Please input layer of the square");
        return true;
    }
    if (lenA <= 0) {
        PRINTE("Length of the square must be positive");
        return true;
    }
    if (layerN <= 0) {
        PRINTE("Layer of the square must be positive");
        return true;
    }
    if (cmd[OUTPUT] && output) {
        if (strcmp(output + strlen(output) - 4, ".bmp")) {
            PRINTE("Output file name must be bmp");
            return true;
        }
    }
    return false;
}

void setWhite(sbPict **arr, int h, int w) {
    for (int hdx = 0; hdx < h; hdx++)
        for (int wdx = 0; wdx < w; wdx++) arr[hdx][wdx] = (sbPict){255, 255, 255};
}

void drawSquare(sbPict **arr, int posX, int posY, int lenX, int lenY) {
    for (int hdx = posY; hdx < posY + lenY; hdx++)
        for (int wdx = posX; wdx < posX + lenX; wdx++) arr[hdx][wdx] = (sbPict){0, 0, 0};

    return;
}

int gRate(int len) {
    return (int)len * (1 + sqrt(5)) / 2;
}

int drawGoldenRatio(int len, int layer, char *fileName) {
    int width = gRate(len), height = len;
    sbHead bmpHead = initHead(height, width);
    sbPict **bmpPict = initArr(height, width);
    FILE *fp = fopen(fileName, "wb");

    if (fp == NULL) {
        PRINTE("Open file failed");
        return -1;
    }
    setWhite(bmpPict, height, width);
    drawSquare(bmpPict, 10, 10, 100, 100);

    while (layer--) {
    }

    if (ArrtoBMP(&fp, bmpPict, bmpHead)) {
        PRINTE("Write file failed");
        return -1;
    }
    fclose(fp);
    freeArr(bmpPict, len);
    return 0;
}

int main(int argc, char *argv[]) {
    bool cmd[4] = {false};
    int opt, lenA, layerN;
    char *output = NULL;

    while ((opt = getopt_long(argc, argv, "a:n:o:h", long_options, NULL)) != -1) {
        if (opt == 'o') output = optarg, cmd[OUTPUT] = true;
        if (opt == 'a') lenA = atoi(optarg), cmd[LEN_A] = true;
        if (opt == 'n') layerN = atoi(optarg), cmd[LAYER_N] = true;
        if (opt == 'h') printHelp(), cmd[HELP] = true;
        if (opt == '?') {
            PRINTE("Invalid option %s", argv[optind - 1]);
            return -1;
        }
    }
    if (isInvalid(cmd, lenA, layerN, output)) return -1;
    if (!cmd[OUTPUT]) output = "output.bmp";
    if (drawGoldenRatio(lenA, layerN, output)) {
        PRINTE("Draw golden ratio failed");
        return -1;
    }

    return 0;
}
