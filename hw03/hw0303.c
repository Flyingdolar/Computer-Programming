#include <getopt.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "bmp.h"

struct option long_options[] = {
    {"posx", 0, NULL, 'x'},
    {"posy", 0, NULL, 'y'},
    {"width", 0, NULL, 'w'},
    {"height", 0, NULL, 'h'},
    {"number", 0, NULL, 'n'},
    {"input", 0, NULL, 'i'},
    {"output", 0, NULL, 'o'},
    {"help", 0, NULL, 'H'},
    {NULL, 0, NULL, 0}};

typedef enum _opts_ {
    POSX,
    POSY,
    WIDTH,
    HEIGHT,
    INPUT,
    OUTPUT,
    NUMBER,
    HELP
} opts;

void printHelp() {
    printf("HW0303 - Mosaic\n");
    printf("Usage & option\n");
    printf("\n");
    printf("  -x, --posx\t\tmandatory, x value\n");
    printf("  -y, --posy\t\tmandatory, y value\n");
    printf("  -w, --width\t\tmandatory, w value\n");
    printf("  -h, --height\t\tmandatory, h value\n");
    printf("  -n, --number\t\toptional, n value. Default n is 2\n");
    printf("  -i, --input\t\tmandatory, the input BMP file name\n");
    printf("  -o, --output\t\toptional, the output BMP file name. Default name is output.bmp\n");
    printf("  -H, --help\t\tprint this help\n");
    return;
}

bool isInvalid(bool cmd[], sPos pos, char *input_file, char *output_file) {
    if (cmd[HEIGHT] == false || cmd[WIDTH] == false) {
        PRINT_E("Missing mandatory option: -w or -h");
        return true;
    }
    if (cmd[POSX] == false || cmd[POSY] == false) {
        PRINT_E("Missing mandatory option: -x or -y");
        return true;
    }
    if (cmd[INPUT] == false || cmd[OUTPUT] == false) {
        PRINT_E("Missing mandatory option: -i or -o");
        return true;
    }
    if (input_file == NULL || output_file == NULL) {
        PRINT_E("Please specify input file and output file");
        return true;
    }
    if (pos.w <= 0 || pos.h <= 0) {
        PRINT_E("Invalid width or height");
        return true;
    }
    if (pos.x < 0 || pos.y < 0) {
        PRINT_E("Invalid position");
        return true;
    }
    if (pos.num <= 0) {
        PRINT_E("Invalid number");
        return true;
    }
    return false;
}

int main(int argc, char *argv[]) {
    bool cmd[8] = {false};
    FILE *fb = NULL;
    sbPict **pict;
    sbHead header;
    sPos pos = {0, 0, 0, 0, 50};
    char opt, *input_file, *output_file;

    while ((opt = getopt_long(argc, argv, "x:y:w:h:i:o:n:H", long_options, NULL)) != -1) {
        if (opt == 'x') cmd[POSX] = true, pos.x = atoi(optarg);
        if (opt == 'y') cmd[POSY] = true, pos.y = atoi(optarg);
        if (opt == 'w') cmd[WIDTH] = true, pos.w = atoi(optarg);
        if (opt == 'h') cmd[HEIGHT] = true, pos.h = atoi(optarg);
        if (opt == 'n') cmd[NUMBER] = true, pos.num = atoi(optarg);
        if (opt == 'i') cmd[INPUT] = true, input_file = optarg;
        if (opt == 'o') cmd[OUTPUT] = true, output_file = optarg;
        if (opt == 'H') cmd[HELP] = true, printHelp();
        if (opt == '?') {
            printf("Unknown option: %c\n", optopt);
            return -1;
        }
    }
    printf("%d %d %d %d %d\n", pos.x, pos.y, pos.w, pos.h, pos.num);
    if (optind != argc) input_file = argv[optind];
    if (cmd[HELP]) return 0;
    if (isInvalid(cmd, pos, input_file, output_file)) return -1;

    fb = fopen(input_file, "rb");
    if (fb == NULL) {
        PRINT_E("Cannot open file: %s", input_file);
        return -1;
    }
    pict = BMPtoArr(fb, &header);
    if (pict == NULL) return -1;
    fclose(fb);

    if (drawMosaic(pict, header, pos) == -1) return -1;

    fb = fopen(output_file, "wb");
    if (fb == NULL) {
        PRINT_E("Cannot open file: %s", output_file);
        return -1;
    }
    if (ArrtoBMP(&fb, pict, header) == -1) return -1;
    freeArr(pict, header.h);
    fclose(fb);

    return 0;
}
