#include <stdio.h>

int main() {
    int a, b;
    char str[1000], ntr[1000], trash[1000];

    scanf("{\"chapter\":%d,", &a);
    scanf("\"verse\":%d,", &b);
    scanf("\"text\":\"%[^\"]\",", str);
    scanf("\"translation_id\":\"%[^\"]\",", trash);
    scanf("\"book_id\":\"%[^\"]\",", trash);
    scanf("\"book_name\":\"%[^\"]\"}", ntr);

    printf("a = %d\n", a);
    printf("b = %d\n", b);
    printf("str = %s\n", str);
    printf("ntr = %s\n", ntr);
    return 0;
}
