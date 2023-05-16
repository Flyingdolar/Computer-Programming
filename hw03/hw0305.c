#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "midi.h"

#define PRINT_E(...) \
    printf("\033[0;31mE: "), printf(__VA_ARGS__), printf("\n\033[0m")

#ifdef DEBUG
#define PRINT_D(...) printf("\033[0;32m"), printf(__VA_ARGS__), printf("\033[0m")
#else
#define PRINT_D(...)
#endif

int errName(char *inStr, char *outStr) {
    int len = strlen(inStr);
    if (len < 4 || strcmp(inStr + len - 4, ".txt") != 0) {
        PRINT_E("Input file name must end with \".txt\".");
        return -1;
    }
    len = strlen(outStr);
    if (len < 4 || strcmp(outStr + len - 4, ".mid") != 0) {
        PRINT_E("Output file name must end with \".mid\".");
        return -1;
    }
    return 0;
}

int main(void) {
    char inStr[1000], outStr[1000];
    FILE *inFile, *outFile;

    printf("Welcome to Cool Sound Infinity Exporter!\n");
    printf("Please enter input and output file name.\n");
    printf("Input file name: "), scanf("%s", inStr);
    printf("Output file name: "), scanf("%s", outStr);

    if (errName(inStr, outStr) == -1) return -1;

    inFile = fopen(inStr, "rb");
    outFile = fopen(outStr, "wb");

    if (inFile == NULL) {
        PRINT_E("Failed to open input file.");
        return -1;
    }
    if (outFile == NULL) {
        PRINT_E("Failed to create output file.");
        return -1;
    }

    if (parseMidi(inFile, outFile) == -1) {
        PRINT_E("Failed to parse MIDI file.");
        return -1;
    }

    fclose(inFile);
    fclose(outFile);
    return 0;
}
