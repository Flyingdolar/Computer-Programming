#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "insert.h"

int main() {
    char *string = "hello world";
    char *insert = "big";
    char *result;

    strinsert(&result, string, 5, insert);
    printf("%s\n", result);
    return 0;
}
