#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int32_t strinsert(char **ppResult, const char *pStr1, int32_t location, const char *pStr2) {
    if (location < 0 || location > strlen(pStr1)) return -1;
    if (ppResult == NULL || *ppResult != NULL) return -1;

    size_t len = strlen(pStr1) + strlen(pStr2) + 1;
    *ppResult = (char *)malloc(len * sizeof(char));

    for (size_t idx = 0; idx < len - 1; idx++) {
        if (idx < location)
            (*ppResult)[idx] = pStr1[idx];
        else if (idx < location + strlen(pStr2))
            (*ppResult)[idx] = pStr2[idx - location];
        else
            (*ppResult)[idx] = pStr1[idx - strlen(pStr2)];
    }
    (*ppResult)[len - 1] = '\0';
    printf("%s\n", (*ppResult));
    return 0;
}
