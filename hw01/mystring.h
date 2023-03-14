#pragma once
#include <stdint.h>
#include <stdio.h>
#include <string.h>

char *mystrchr(const char *string, int character);
char *mystrrchr(const char *string, int character);
size_t mystrspn(const char *string, const char *accept);
size_t mystrcspn(const char *string, const char *reject);
char *mystrpbrk(const char *string, const char *accept);
char *mystrstr(const char *haystack, const char *needle);
char *mystrtok(char *string, const char *delimiter);
