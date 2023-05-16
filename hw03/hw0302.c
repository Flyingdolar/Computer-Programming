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

typedef enum _opts_ {
    ASCEND,
    DESCEND,
    HELP
} opts;

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
    if (cmd[ASCEND] && cmd[DESCEND]) {
        PRINT_E("Please specify only one option: -a or -d");
        return true;
    }
    if (input_file == NULL) {
        PRINT_E("Please specify input file");
        return true;
    }
    return false;
}

int main(const int argc, char *const argv[]) {
    bool cmd[3] = {false};
    FILE *fp = NULL;
    pFile fTree;
    int opt;
    char *input_file = NULL;

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
        PRINT_E("Cannot open file: %s", input_file);
        return -1;
    }
    if (isZIP(input_file, fp) == false) {
        PRINT_E("Not a ZIP file: %s", input_file);
        return -1;
    }

    // 抓取檔案名稱，並存成一棵樹（內含有解析 Zip 的功能函式）
    fTree = createTree(fp);
    // 依據指令排序樹（ cmpAsc 是正序排列的比較方式、 cmpDsc 是反序排列的比較方式）
    sortTree(fTree, cmpAsc);
    if (cmd[DESCEND]) sortTree(fTree, cmpDsc);
    // 設定樹狀結構的縮排
    setIndent(fTree);
    // 印出樹狀結構
    printTree(fTree);
    // 釋放樹狀結構的記憶體
    free_node(&fTree);
    fclose(fp);

    return 0;
}
