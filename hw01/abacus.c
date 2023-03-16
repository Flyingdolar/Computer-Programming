#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "insert.h"

typedef struct _sAbacus {
    uint8_t number;
    uint8_t *pUpperRod;
    uint8_t *pLowerRod;
} sAbacus;

int16_t to_int(char ltr) {
    if (ltr < '0' || ltr > '9') return -1;
    return ltr - '0';
}

char to_char(int16_t val) {
    if (val < 0 || val > 9) return '\0';
    return val + '0';
}

uint8_t *u8t_init(int16_t cNum) {
    uint8_t *new = (uint8_t *)malloc(cNum * sizeof(uint8_t));
    return new;
}

sAbacus *abacus_init(void) {
    sAbacus *new = (sAbacus *)malloc(sizeof(sAbacus));
    return new;
}

void abacus_free(sAbacus *Abacus) {
    if (Abacus) free(Abacus);
    Abacus = NULL;
    return;
}

int8_t abacus_col(sAbacus sA, uint8_t idx) {
    int32_t uppRod = sA.pUpperRod[idx];
    int32_t lowRod = sA.pLowerRod[idx];

    if (uppRod < 0 || uppRod > 1) return -1;
    if (lowRod < 0 || lowRod > 4) return -1;
    return uppRod * 5 + lowRod;
}

int8_t abacus_check(sAbacus sA) {
    if (!(sA.number)) return -1;
    if (sA.pLowerRod[0] == 0 && sA.pUpperRod[0] == 0) {
        if (sA.number == 1) return 0;
        return -1;
    }

    for (int idx = 0; idx < sA.number; idx++) {
        if (sA.pLowerRod[idx] > 4) return -1;
        if (sA.pUpperRod[idx] > 1) return -1;
    }
    return 0;
}

int8_t abacus_sort(sAbacus **ppA_bg, sAbacus **ppA_sm) {
    sAbacus *pA = *ppA_bg;
    uint8_t A1_col, A2_col;

    if ((*ppA_bg)->number > (*ppA_sm)->number) return 0;
    if ((*ppA_sm)->number > (*ppA_bg)->number) {
        *ppA_bg = *ppA_sm;
        *ppA_sm = pA;
        return 0;
    };

    for (int idx = 0; idx < pA->number; idx++) {
        A1_col = abacus_col(**ppA_bg, idx);
        A2_col = abacus_col(**ppA_sm, idx);

        if (A1_col > A2_col) return 0;
        if (A2_col > A1_col) {
            *ppA_bg = *ppA_sm;
            *ppA_sm = pA;
            return 0;
        }
    }
    return 1;
}

char *abacus_str(sAbacus sA) {
    char *string = (char *)malloc((sA.number + 1) * sizeof(char));

    for (int idx = 0; idx < sA.number; idx++) {
        // 將每列算盤內容：取值、轉換為文字
        string[idx] = to_char(abacus_col(sA, idx));
        if (string[idx] == '\0') return NULL;
    }
    string[sA.number] = '\0';
    return string;
}

int32_t abacus_set(sAbacus *pA, const char *str) {
    if (strlen(str) <= 0 || strlen(str) > 255) return -1;
    pA->number = strlen(str);
    pA->pUpperRod = u8t_init(pA->number);
    pA->pLowerRod = u8t_init(pA->number);

    if (str[0] == '0') {  // 首位數字為 0 的情況
        if (pA->number > 1) return -1;
        pA->pLowerRod[0] = 0;
        pA->pUpperRod[0] = 0;
        return 0;
    }

    for (int16_t idx = 0; idx < pA->number; idx++) {
        int16_t deg = to_int(str[idx]);
        if (deg < 0) return -1;
        if (deg > 4) {
            pA->pUpperRod[idx] = 1;
            pA->pLowerRod[idx] = deg - 5;
        } else {
            pA->pUpperRod[idx] = 0;
            pA->pLowerRod[idx] = deg;
        }
    }
    return 0;
};

int32_t abacus_add(sAbacus *res, sAbacus sA_1, sAbacus sA_2) {
    if (res == NULL) return -1;
    if (abacus_check(sA_1) == -1) return -1;  // Check if sA_1 is legal
    if (abacus_check(sA_2) == -1) return -1;  // Check if sA_2 is legal

    // Sort to know sA_1, sA_2 which is bigger in pA_big, pA_small
    sAbacus *pA_bg = &sA_1, *pA_sm = &sA_2;
    abacus_sort(&pA_bg, &pA_sm);

    char *bgStr = abacus_str(*pA_bg), *smStr = abacus_str(*pA_sm);
    char letr[2] = {0}, *reStr = (char *)malloc(sizeof(char));

    uint8_t sum, cary = 0;
    for (uint8_t idx = 1; idx <= pA_bg->number; idx++) {
        int16_t iB = pA_bg->number - idx;
        int16_t iS = pA_sm->number - idx;

        sum = to_int(bgStr[iB]) + cary;
        if (iS >= 0) sum += to_int(smStr[iS]);
        cary = sum / 10;
        sum = sum % 10;

        letr[0] = to_char(sum);
        strinsert(&reStr, reStr, 0, letr);
    }
    if (cary) strinsert(&reStr, reStr, 0, "1");
    abacus_set(res, reStr);

    return 0;
};

int32_t abacus_del(sAbacus *res, sAbacus sA_1, sAbacus sA_2) {
    sAbacus *pA_bg = &sA_1, *pA_sm = &sA_2;
    if (abacus_sort(&pA_bg, &pA_sm)) return abacus_set(res, "0");
    if (pA_bg != &sA_1 || pA_sm != &sA_2) return -1;  // Detect wheter A1 is bigger than A2 ?
    char *bgStr = abacus_str(*pA_bg), *smStr = abacus_str(*pA_sm);
    char letr[2] = {0}, *reStr = (char *)malloc(sizeof(char));

    int16_t sum, brow = 0;
    for (uint8_t idx = 1; idx <= pA_bg->number; idx++) {
        int16_t iB = pA_bg->number - idx;
        int16_t iS = pA_sm->number - idx;

        sum = to_int(bgStr[iB]) - brow;
        brow = 0;
        if (iS >= 0) sum -= to_int(smStr[iS]);
        if (sum < 0) brow = 1, sum += 10;
        letr[0] = to_char(sum);
        strinsert(&reStr, reStr, 0, letr);
    }
    while (*reStr == '0') reStr++;
    abacus_set(res, reStr);
    return 0;
}

int32_t abacus_print(const sAbacus Abacus) {
    int16_t index = 0;
    const char *printSpace = "=  -     =";
    const char *printDivide = "=||-|||||=";
    const char *printBorder = "O||||||||O";
    const char *printNum[10] = {
        "=* - ****=", "=* -* ***=", "=* -** **=", "=* -*** *=", "=* -**** =",
        "= *- ****=", "= *-* ***=", "= *-** **=", "= *-*** *=", "= *-**** ="};

    for (int pos = 0; pos < 10; pos++) {
        for (int index = 0; index < Abacus.number; index++) {
            // print 算盤外觀
            if (index == 0) printf("%c", printBorder[pos]);
            if (index > 0) printf("%c", printDivide[pos]);
            printf("%c", printSpace[pos]);
            // 調整顏色
            if (pos && pos < 3) printf("\033[;31;1m");
            if (pos > 3 && pos < 9) printf("\033[;33;1m");
            // print 算盤內容
            int16_t value = abacus_col(Abacus, index);
            if (value < 0) return -1;
            printf("%c", printNum[value][pos]);
            // 調整顏色
            printf("\033[0m");
            // print 算盤外觀
            printf("%c", printSpace[pos]);
            if (index == Abacus.number - 1) printf("%c", printBorder[pos]);
        }
        printf("\n");
    }
    printf("\033[0m");
    return 0;
}
