// hw0101.c
#include <stdio.h>
#include <string.h>

#include "mystring.h"

int main() {
    char str1[] = "Hello world";
    char str2[] = "banana";
    char str3_1[] = "This is a string";
    char str3_2[] = "This is a string";
    char *result;
    size_t len;

    // Test mystrchr
    result = strchr(str1, 'o');
    printf(" strchr: %s\n", result);
    result = mystrchr(str1, 'o');
    printf("mystrchr: %s\n\n", result);

    // Test mystrrchr
    result = strrchr(str1, 'l');
    printf("  strrchr: %s\n", result);
    result = mystrrchr(str1, 'l');
    printf("mystrrchr: %s\n\n", result);

    // Test mystrspn
    len = strspn(str2, "an");
    printf("  strspn: %lu\n", len);
    len = mystrspn(str2, "an");
    printf("mystrspn: %lu\n\n", len);

    // Test mystrcspn
    len = strcspn(str2, "n");
    printf("  strcspn: %lu\n", len);
    len = mystrcspn(str2, "n");
    printf("mystrcspn: %lu\n\n", len);

    // Test mystrpbrk
    result = strpbrk(str2, "na");
    printf("  strpbrk: %s\n", result);
    result = mystrpbrk(str2, "na");
    printf("mystrpbrk: %s\n\n", result);

    // Test mystrstr
    result = strstr(str3_1, "is");
    printf("  strstr: %s\n", result);
    result = mystrstr(str3_2, "is");
    printf("mystrstr: %s\n\n", result);

    // Test mystrtok
    char *token_1 = strtok(str3_1, " ");
    printf("strtok:\n");
    while (token_1 != NULL) {
        printf("  %s\n", token_1);
        token_1 = strtok(NULL, " ");
    }
    char *token_2 = mystrtok(str3_2, " ");
    printf("mystrtok:\n");
    while (token_2 != NULL) {
        printf("  %s\n", token_2);
        token_2 = mystrtok(NULL, " ");
    }

    return 0;
}
