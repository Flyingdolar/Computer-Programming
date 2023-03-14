#include <stdio.h>
#include <string.h>

char **test(const char **input) {
    char wordOne = **input;  // 這行是對的
    char wordTwo = (char)**input;

    char *ptrOne = *input;  // 這行會 Warining 報錯
    char *ptrTwo = (char *)*input;

    char **adrOne = input;  // 這行會 Warining 報錯
    char **adrTwo = (char **)input;

    return input;  // 這行會 Warning 報錯
}

int main() {
    return 0;
}
