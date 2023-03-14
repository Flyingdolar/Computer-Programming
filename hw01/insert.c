#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int32_t strinsert(char **ppResult, const char *pStr1, int32_t location, const char *pStr2) {
    char *ptrIn = (char *)malloc((strlen(pStr1) + strlen(pStr2)) * sizeof(char));
    *ppResult = ptrIn;
    while (location--) {
        *ptrIn = *pStr1;
        ptrIn++;
        pStr1++;
    }
    while (*pStr2) {
        *ptrIn = *pStr2;
        ptrIn++;
        pStr2++;
    }
    while (*pStr1) {
        *ptrIn = *pStr1;
        ptrIn++;
        pStr1++;
    }
    return 0;
}
