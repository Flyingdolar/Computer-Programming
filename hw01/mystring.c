#include <stdint.h>
#include <stdio.h>
#include <string.h>

char *glob_str;

char *mystrchr(const char *string, int character) {
    while (*string) {
        if (*string == character) return ((char *)string);
        string++;
    }
    return NULL;
};

char *mystrrchr(const char *string, int character) {
    char *result = NULL;
    while (*string) {
        if (*string == character) result = (char *)string;
        string++;
    }
    return result;
};

size_t mystrspn(const char *string, const char *accept) {
    size_t result = 0;
    int8_t ascii[256] = {0};
    while (*accept) {
        ascii[*accept] = 1;
        accept++;
    }
    while (*string) {
        if (!ascii[*string]) return result;
        string++;
        result++;
    }
    return result;
};

size_t mystrcspn(const char *string, const char *reject) {
    size_t result = 0;
    int8_t ascii[256] = {0};
    while (*reject) {
        ascii[*reject] = 1;
        reject++;
    }
    while (*string) {
        if (ascii[*string]) return result;
        string++;
        result++;
    }
    return result;
};

char *mystrpbrk(const char *string, const char *accept) {
    return (char *)(&string[mystrcspn(string, accept)]);
};

char *mystrstr(const char *haystack, const char *needle) {
    char *strPtr, *keyPtr;
    while (*haystack) {
        strPtr = (char *)haystack;
        keyPtr = (char *)needle;
        while (*keyPtr) {
            if (*strPtr != *keyPtr) break;
            strPtr++;
            keyPtr++;
        }
        if (!(*keyPtr)) break;
        haystack++;
    }
    if (*haystack)
        return (char *)haystack;
    else
        return NULL;
};

int8_t patch(const char *string, const char *accept) {
    while (*accept) {
        if (*string == *accept) return 1;
        accept++;
    }
    return 0;
}

char *mystrtok(char *string, const char *delimiter) {
    char *deliPtr = (char *)delimiter;
    if (string)
        glob_str = string;
    else
        string = glob_str;
    while (!patch(glob_str, delimiter)) {
        if (!(*glob_str)) {
            if (*string)
                return string;
            else
                return NULL;
        };
        glob_str++;
    }
    while (patch(glob_str, delimiter)) {
        if (!(*glob_str)) {
            glob_str = NULL;
            break;
        };
        *glob_str = 0;
        glob_str++;
    }
    return string;
};
