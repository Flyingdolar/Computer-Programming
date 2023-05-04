#include <getopt.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "zip.h"

struct option long_options[] = {
    {"ascend", 0, NULL, 'a'},
    {"descend", 0, NULL, 'd'},
    {NULL, 0, NULL, 0}};

enum { ASCEND,
       DESCEND,
       HELP };

void printHelp() {
    printf("HW0302 - ZIP File Viewer\n");
    printf("Usage & option\n");
    printf("\n");
    printf("  -a, --ascend\t\tprint file that sorted by ASCII ascend\n");
    printf("  -d, --descend\t\tprint file that sorted by ASCII descend\n");
    printf("  -h, --help\t\tprint this help\n");
    return;
}

bool isInvalid(bool cmd[], char *input_file) {
    printf("\033[0;31m");
    if (cmd[ASCEND] && cmd[DESCEND]) {
        printf("E: Please specify -a or -d\n");
        return true;
    }
    if (input_file == NULL) {
        printf("E: Please specify input file\n");
        return true;
    }
    printf("\033[0m");
    return false;
}

bool isZIP(char *fName, FILE *fp) {
    uint32_t signature = 0;
    fread(&signature, sizeof(uint32_t), 1, fp);
    rewind(fp);
    // if (&fName[strlen(fName) - 3] != "zip") return false;
    if (signature == 0x04034b50) return true;
    return false;
}

void printHead(zipHead *head) {
    printf("Compression method: %d\n", head->compression);
    printf("Last modified date: %d\n", head->mod_date);
    printf("Last modified time: %d\n", head->mod_time);
    printf("CRC-32: %d\n", head->crc32);
    printf("Compressed size: %d\n", head->compressed_size);
    printf("Uncompressed size: %d\n", head->uncompressed_size);
    printf("File name length: %d\n", head->file_name_length);
    printf("Extra field length: %d\n\n", head->extra_field_length);
    return;
}

char *getFileName(FILE **fp) {
    char *file_name = NULL;
    zipHead head;
    uint16_t fileLen, extraLen;

    if (feof(*fp)) return NULL;
    fread(&head, sizeof(zipHead), 1, *fp);

    file_name = (char *)malloc(head.file_name_length + 1);
    fread(file_name, head.file_name_length, 1, *fp);
    file_name[head.file_name_length] = '\0';
    fseek(*fp, head.extra_field_length, SEEK_CUR);
    fseek(*fp, head.compressed_size, SEEK_CUR);

    printf("Name is %s\n", file_name);
    printHead(&head);

    return file_name;
}

int main(const int argc, char *const argv[]) {
    bool cmd[3] = {false};
    FILE *fp = NULL;
    int opt;
    char *input_file = NULL, *file_name = NULL;

    while ((opt = getopt_long(argc, argv, ":a:d:h", long_options, NULL)) != -1) {
        if (input_file == NULL && optarg != NULL) input_file = optarg;
        if (opt == 'a') cmd[ASCEND] = true;
        if (opt == 'd') cmd[DESCEND] = true;
        if (opt == 'h') cmd[HELP] = true, printHelp();
        if (opt == '?') {
            printf("Unknown option: %c\n", optopt);
            return -1;
        }
    }
    if (optind != argc) input_file = argv[optind];
    if (cmd[HELP]) return 0;
    if (isInvalid(cmd, input_file)) return -1;

    fp = fopen(input_file, "rb");
    if (fp == NULL) {
        printf("E: Cannot open file: %s\n", input_file);
        return -1;
    }
    if (isZIP(input_file, fp) == false) {
        printf("E: Not a ZIP file: %s\n", input_file);
        return -1;
    }

    int x = 0;
    while (x < 10) {
        file_name = getFileName(&fp);
        if (file_name == NULL) break;
        free(file_name);
        x += 1;
    }

    return 0;
}
