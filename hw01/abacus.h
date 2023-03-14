#pragma once
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _sAbacus {
    uint8_t number;
    uint8_t *pUpperRod;
    uint8_t *pLowerRod;
} sAbacus;

int32_t abacus_set(sAbacus *ptrAbacus, char *strNumber);

int32_t abacus_add(sAbacus *result, sAbacus origin, sAbacus add);

int32_t abacus_del(sAbacus *result, sAbacus origin, sAbacus del);

int32_t abacus_print(sAbacus Abacus);

char *abacus_getNumber(sAbacus Abacus);
