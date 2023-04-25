#ifndef STRING_H
#define STRING_H
#include <string.h>

char *subaryastrchr(const char *s, int c){
    return strchr(s, c);
}
char *subaryastrrchr(const char *s, int c){
    return strrchr(s, c);
}
size_t subaryastrspn(const char *s, const char *accept){
    return strspn(s, accept);    
}
size_t subaryastrcspn(const char *s, const char *reject){
    return strcspn(s, reject);
}
char *subaryastrpbrk(const char *s, const char *accept){
    return strpbrk(s, accept);
}
char *subaryastrstr(const char *haystack , const char *needle){
    return strstr(haystack, needle);
}
char *subaryastrtok(char *str, const char *delim){
    return strtok(str, delim);
}
int judgeStringCmp(char *str1, char *str2){
    return strcmp(str1, str2);
}
int judgeSizeCmp(size_t srya, size_t stu){
    return srya == stu ? 0 : 1;
}

#endif