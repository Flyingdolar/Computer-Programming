#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

    location = 0;
    strinsert(&result, "", location, "1234");
    printf("Expected output: 1234\n");
    printf("  Actual output: %s\n\n", result);
    free(result);

    char *test = (char *)malloc(sizeof(char));
    location = 0;
    strinsert(&test, test, location, "1234");
    printf("Expected output: 1234\n");
    printf("  Actual output: %s\n\n", test);
    printf("Length: %ld\n", strlen(test));
    free(result);

    return 0;
}
