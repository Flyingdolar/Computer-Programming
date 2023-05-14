#pragma once
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define PRINT_E(...) \
    printf("\033[0;31mE: "), printf(__VA_ARGS__), printf("\n\033[0m")

#ifdef DEBUG
#define PRINT_D(...) printf("\033[0;32m"), printf(__VA_ARGS__), printf("\033[0m")
#else
#define PRINT_D(...)
#endif

// zip_file_header in atrribute packed
typedef struct zip_file_header {
    uint32_t signature;
    uint16_t version;
    uint16_t flags;
    uint16_t compression;
    uint16_t mod_time;
    uint16_t mod_date;
    uint32_t crc32;
    uint32_t compressed_size;
    uint32_t uncompressed_size;
    uint16_t file_name_length;
    uint16_t extra_field_length;
} __attribute__((packed)) zipHead;

typedef enum _file_type_ {
    THEAD,
    TFILE,
    TDIR,
} fileType;

typedef struct _file_ *pFile;
typedef struct _file_ {
    char *name;
    int len, layer, indt;
    fileType type;
    pFile Prev, Next;
    pFile Parent, Child;
} sFile;

// ZIP function
bool isZIP(char *fName, FILE *fp);
char *getFileName(FILE **fp);

// Tree function
pFile createTree(FILE *fp);
void sortTree(pFile root, int32_t (*cond)(pFile, pFile));
void printTree(pFile root);

// String function
char **splitPath(char *path, int *len);
void freePath(char **path, int len);

// Node function
pFile init_node(fileType type);
void free_node(pFile *f);
void insert_node(pFile head, pFile node);
pFile search_node(pFile head, char *name);

// Sort function
void mgSort(pFile Arr, int32_t (*cond)(pFile, pFile));
int32_t cmpAsc(pFile a, pFile b);
int32_t cmpDsc(pFile a, pFile b);
int32_t cmpType(pFile a, pFile b);
