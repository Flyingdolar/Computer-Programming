#include <stdint.h>
#include <stdio.h>

#include "insert.h"

int main() {
    char *result = NULL;
    int32_t location = 1;

    strinsert(&result, "Hello", location, "ABC");
    printf("Expected output: HABCello\n");
    printf("  Actual output: %s\n\n", result);
    free(result);

    location = 0;
    strinsert(&result, "World", location, "123");
    printf("Expected output: 123World\n");
    printf("  Actual output: %s\n\n", result);
    free(result);

    location = 5;
    strinsert(&result, "Testing", location, "1234");
    printf("Expected output: Testi1234ng\n");
    printf("  Actual output: %s\n\n", result);
    free(result);

    return 0;
}
