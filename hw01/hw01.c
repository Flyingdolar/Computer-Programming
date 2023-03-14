#include <stdio.h>
#include <string.h>

#include "mystring.h"

int main() {
    char string[] =
        "Chan x chan Ai! chiki chiki bam bam\n"
        "Ai? chiki chiki chan chan\n"
        "Aa! chiki chiki ban ban\n"
        "Ai! chuki chuki chuki ban ban\n"
        "High chiki chiki bam bam\n"
        "Ai? chiki chiki chan chan\n"
        "Aa! chiki chiki ban ban\n"
        "Ai! chuki chuki";

    char keyC = 'c', *space = " x?!\n", keyWord[] = "chiki";
    char *ptr;
    char letter[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz ";
    char quotation[] = ".,/*&^@#$()-_!?";

    // Test "mystrchr"
    printf("結果一：\n%s\n\n", mystrchr(string, keyC));

    // Test "mystrrchr"
    printf("結果二：\n%s\n\n", mystrrchr(string, keyC));

    // Test "mystrspn"
    printf("結果三：%lu\n\n", mystrspn(string, letter));

    // Test "mystrcspn"
    printf("結果四：%lu\n\n", mystrcspn(string, quotation));

    // Test "mystrpbrk"
    printf("結果五：\n%s\n\n", mystrpbrk(string, quotation));

    // Test "mystrstr"
    printf("結果六：\n%s\n\n", mystrstr(string, keyWord));

    // Test "mystrtok"
    ptr = mystrtok(string, space);
    printf("結果七：\n%s\n", ptr);
    while (ptr) {
        ptr = mystrtok(NULL, space);
        printf("%s\n", ptr);
    }

    return 0;
}
