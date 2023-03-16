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

int32_t abacus_set(sAbacus *ptrAbacus, const char *strNumber);

int32_t abacus_add(sAbacus *result, const sAbacus origin, const sAbacus add);

int32_t abacus_del(sAbacus *result, const sAbacus origin, const sAbacus del);

int32_t abacus_print(const sAbacus Abacus);

char *abacus_str(const sAbacus Abacus);
