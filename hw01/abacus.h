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

sAbacus *abacus_init(void);

void abacus_free(sAbacus *Abacus);

char *abacus_str(sAbacus sA);

int32_t abacus_set(sAbacus *pA, const char *str);

int32_t abacus_add(sAbacus *res, sAbacus sA_1, sAbacus sA_2);

int32_t abacus_del(sAbacus *res, sAbacus sA_1, sAbacus sA_2);

int32_t abacus_print(sAbacus sA);
