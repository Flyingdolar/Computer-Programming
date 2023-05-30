#include <getopt.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "bmp.h"

#define PRINTE(...) printf("\033[0;31mE: "), printf(__VA_ARGS__), printf("\033[0m\n")

typedef struct _fileHeader_ sfHead;
struct _fileHeader_ {
    uint32_t nameSize;
    uint32_t fileSize;
} __attribute__((__packed__));

struct option long_options[] = {
    {"insert", 0, NULL, 'i'},
    {"extract", 0, NULL, 'e'},
    {"help", 0, NULL, 'h'},
    {NULL, 0, NULL, 0}};

typedef enum _optCmd_ {
    INSERT,
    EXTRACT,
    HELP
} optCmd;

void printHelp() {
    printf("\033[0;33mUsing Help Command...\033[0m\n");
    printf("+===========================+\n");
    printf("| HW0404 - Data Hider (BMP) |\n");
    printf("+===========================+\n");
    printf("It is a tool help you hide data in BMP file\n");

    printf("\n* Usages *\n");
    printf("hw0401\t[Options]...\t[BMP File]\n");
    printf("\n* Options *\n");
    printf(" %s / %-10s\t | %-15s\t | %-20s\n", "Short", "Long Opt.", "Variable (Default)", "Function");
    printf("-------------------------+-----------------------+---------------------------------\n");
    printf(" %3s    %-10s\t | %-15s\t | %-20s\n", "-i", "--insert", "[File]", "Insert and hide [File] into [BMP File]");
    printf(" %3s    %-10s\t | %-15s\t | %-20s\n", "-e", "--extract", "", "Extract all hidden file from [BMP File]");
    printf(" %3s    %-10s\t | %-15s\t | %-20s\n", "-h", "--help", "", "Print this help");
    return;
}

bool isInvalid(bool cmd[], char *fileName, char *bmpName) {
    if (cmd[INSERT] && fileName == NULL) {
        PRINTE("Insert without specify file name");
        return true;
    }
    if (bmpName == NULL) {
        PRINTE("No BMP file specify");
        return true;
    }
    if (cmd[INSERT] && cmd[EXTRACT]) {
        PRINTE("Cannot insert and extract at the same time");
        return true;
    }
    if (!cmd[INSERT] && !cmd[EXTRACT]) {
        PRINTE("No command specify");
        return true;
    }
    if (strncmp(bmpName + strlen(bmpName) - 4, ".bmp", 4) != 0) {
        PRINTE("BMP file name must end with .bmp");
        return true;
    }
    return false;
}

int checkBMP(char *bmpName) {
    sbHead header;
    FILE *fp = fopen(bmpName, "rb");

    if (fp == NULL) {
        PRINTE("Cannot open BMP file");
        return -1;
    }
    fread(&header, sizeof(sbHead), 1, fp);
    if (strncmp(header.bm, "BM", 2) != 0) {
        PRINTE("Not a BMP file");
        return -1;
    }
    fclose(fp);
    return 0;
}

int insertData(char *bmpName, char *fileName) {
    FILE *bmp = fopen(bmpName, "ab");
    FILE *file = fopen(fileName, "rb");
    sfHead fHead;

    if (file == NULL) {
        PRINTE("Cannot open file %s", fileName);
        return -1;
    }
    fseek(file, 0, SEEK_END);
    fHead.nameSize = strlen(fileName) + 1;
    fHead.fileSize = ftell(file);

    fseek(file, 0, SEEK_SET);
    fwrite(&fHead, sizeof(sfHead), 1, bmp);
    fwrite(fileName, fHead.nameSize, 1, bmp);
    for (int i = 0; i < fHead.fileSize; i++)
        fputc(fgetc(file), bmp);
    fclose(bmp);
    fclose(file);
    return 0;
}

int extractData(char *bmpName) {
    FILE *bmp = fopen(bmpName, "rb"), *file;
    sbHead bmpHead;
    sfHead fileHead;
    char fileName[1024];

    fread(&bmpHead, sizeof(sbHead), 1, bmp);
    fseek(bmp, bmpHead.size, SEEK_SET);
    while (!feof(bmp)) {
        fread(&fileHead, sizeof(sfHead), 1, bmp);
        fread(fileName, fileHead.nameSize, 1, bmp);
        if (feof(bmp)) break;
        file = fopen(fileName, "wb");
        for (int i = 0; i < fileHead.fileSize; i++)
            fputc(fgetc(bmp), file);
        fclose(file);
    }
    fclose(bmp);
    return 0;
}

int main(int argc, char *argv[]) {
    char *bmpName, *fileName;
    bool cmd[3] = {false};
    int opt;

    while ((opt = getopt_long(argc, argv, "i:eh", long_options, NULL)) != -1) {
        if (opt == 'i')
            cmd[INSERT] = true, fileName = strdup(optarg);
        if (opt == 'e')
            cmd[EXTRACT] = true;
        if (opt == 'h')
            cmd[HELP] = true, printHelp();
        if (opt == '?') {
            PRINTE("Unknown option %c\n", optopt);
            return -1;
        }
    }
    if (cmd[HELP]) return 0;
    if (optind < argc) bmpName = strdup(argv[optind]);
    if (isInvalid(cmd, fileName, bmpName)) return -1;
    if (checkBMP(bmpName) != 0) return -1;

    if (cmd[INSERT])
        if (insertData(bmpName, fileName) != 0) return -1;
    if (cmd[EXTRACT])
        if (extractData(bmpName) != 0) return -1;
    return 0;
}
