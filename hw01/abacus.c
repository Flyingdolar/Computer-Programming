#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _sAbacus {
    uint8_t number;
    uint8_t *pUpperRod;
    uint8_t *pLowerRod;
} sAbacus;

int16_t ChartoInt(char letter) {
    if (letter <= '0' && letter >= '9') return -1;
    return letter - '0';
}

char InttoChar(int16_t value) {
    if (value <= 0 && value >= 9) return '\0';
    return value + '0';
}

int16_t getRowNum(sAbacus Abacus, int16_t index) {
    int16_t UpperRod = Abacus.pUpperRod[index];
    int16_t LowerRod = Abacus.pLowerRod[index];
    if (UpperRod < 0 || UpperRod > 1) return -1;
    if (LowerRod < 0 || LowerRod > 4) return -1;
    return UpperRod * 5 + LowerRod;
}

int32_t abacus_max(const sAbacus *Abacus_A, const sAbacus *Abacus_B) {
    if (Abacus_A->number > Abacus_B->number)
        return 1;
    else if ((Abacus_B->number > Abacus_A->number))
        return -1;
    else {
        for (int index = 0; index < Abacus_A->number; index++) {
            if (ChartoInt(getRowNum(*Abacus_A, index)) > ChartoInt(getRowNum(*Abacus_B, index)))
                return 1;
            else if (ChartoInt(getRowNum(*Abacus_B, index)) > ChartoInt(getRowNum(*Abacus_A, index)))
                return -1;
        }
    }
    return 0;
}

int32_t abacus_set(sAbacus *ptrAbacus, char *strNumber) {
    if (strlen(strNumber) > 255) return -1;
    ptrAbacus->number = strlen(strNumber);
    ptrAbacus->pUpperRod = (uint8_t *)malloc(ptrAbacus->number * sizeof(uint8_t));
    ptrAbacus->pLowerRod = (uint8_t *)malloc(ptrAbacus->number * sizeof(uint8_t));

    for (int16_t index = 0; index < ptrAbacus->number; index++) {
        int16_t num = ChartoInt(strNumber[index]);
        if (num < 0)
            return -1;
        else if (num > 4) {
            num -= 5;
            ptrAbacus->pUpperRod[index] = 1;
            ptrAbacus->pLowerRod[index] = num;
        } else {
            ptrAbacus->pUpperRod[index] = 0;
            ptrAbacus->pLowerRod[index] = num;
        }
    }
    return 0;
};

int32_t abacus_add(sAbacus *result, const sAbacus origin, const sAbacus add) {
    int8_t overflow = 0;
    int16_t rIndex, oIndex, aIndex;

    if (result == NULL) return -1;
    result->number = 20;
    if (abacus_max(&origin, &add) >= 0)
        result->number = origin.number;
    else
        result->number = add.number;
    if (origin.number == add.number) {
        if (InttoChar(getRowNum(origin, 0)) + InttoChar(getRowNum(add, 0)) > 9)
            result->number++;
    }

    result->pLowerRod = (uint8_t *)malloc(result->number * sizeof(uint8_t));
    result->pUpperRod = (uint8_t *)malloc(result->number * sizeof(uint8_t));

    for (int16_t index = 1; index <= result->number; index++) {
        rIndex = result->number - index;
        oIndex = origin.number - index;
        aIndex = add.number - index;

        result->pUpperRod[rIndex] = 0;
        result->pLowerRod[rIndex] = overflow;
        overflow = 0;

        if (oIndex >= 0) {
            result->pLowerRod[rIndex] += origin.pLowerRod[oIndex];
            result->pUpperRod[rIndex] += origin.pUpperRod[oIndex];
        }
        if (aIndex >= 0) {
            result->pLowerRod[rIndex] += add.pLowerRod[aIndex];
            result->pUpperRod[rIndex] += add.pUpperRod[aIndex];
        }
        if (result->pLowerRod[rIndex] >= 5) {
            result->pLowerRod[rIndex] %= 5;
            result->pUpperRod[rIndex]++;
        }
        if (result->pUpperRod[rIndex] >= 2) {
            result->pUpperRod[rIndex] %= 2;
            overflow = 1;
        }
    }
    return 0;
};

int32_t abacus_del(sAbacus *result, sAbacus origin, sAbacus del) {
    return 0;
};

int32_t abacus_print(sAbacus Abacus) {
    int16_t index = 0;
    const char *printSpace = "=  -     =";
    const char *printDivide = "=||-|||||=";
    const char *printBorder = "O||||||||O";
    const char *printNum[10] = {
        "=* - ****=", "=* -* ***=", "=* -** **=", "=* -*** *=", "=* -**** =",
        "= *- ****=", "= *-* ***=", "= *-** **=", "= *-*** *=", "= *-**** ="};

    for (int pos = 0; pos < 10; pos++) {
        for (int index = 0; index < Abacus.number; index++) {
            if (index == 0) printf("%c", printBorder[pos]);
            if (index > 0) printf("%c", printDivide[pos]);
            printf("%c", printSpace[pos]);
            if (pos && pos < 3) printf("\033[;31;1m");
            if (pos > 3 && pos < 9) printf("\033[;33;1m");
            int16_t value = getRowNum(Abacus, index);
            if (value < 0) return -1;
            printf("%c", printNum[value][pos]);
            printf("\033[0m");
            printf("%c", printSpace[pos]);
            if (index == Abacus.number - 1) printf("%c", printBorder[pos]);
        }
        printf("\n");
    }
    printf("\033[0m");
    return 0;
}

char *abacus_getNumber(sAbacus Abacus) {
    char *string = (char *)malloc((Abacus.number + 1) * sizeof(char));
    for (int index = 0; index < Abacus.number; index++) {
        string[index] = InttoChar(getRowNum(Abacus, index));
        if (string[index] == '\0') return NULL;
    }
    string[Abacus.number] = '\0';
    return string;
}
