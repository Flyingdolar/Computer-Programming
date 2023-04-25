#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 1000

typedef struct _section_ {
    int32_t ch;
    int32_t ver;
    char text[10000];
    char book[1000];
    char trans_id[10000];
    char book_id[1000];
} section;

int main() {
    char input[BUFFER_SIZE], bible[BUFFER_SIZE];
    int32_t num = 0;

    FILE* bFile = fopen("bible.txt", "r");
    if (bFile == NULL) return -1;

    printf("Please enter the search target: ");
    fgets(input, BUFFER_SIZE, stdin);
    input[strlen(input) - 1] = 0;

    for (int idx = 0; input[idx] != 0; idx++)
        input[idx] = tolower(input[idx]);
    printf("%s\n", input);

    while (fgets(bible, BUFFER_SIZE, bFile)) {
        section content;
        char format_string[] =
            "{"
            "\"chapter\":%d,"
            "\"verse\":%d,"
            "\"text\":\"%[^\"]\","
            "\"translation_id\":\"%[^\"]\","
            "\"book_id\":\"%[^\"]\","
            "\"book_name\":\"%[^\"]\"}\n";
        sscanf(
            bible, format_string, &content.ch, &content.ver,
            content.text, content.trans_id, content.book_id, content.book);

        char* lowText = strdup(content.text);
        for (int idx = 0; lowText[idx]; idx++)
            lowText[idx] = tolower(lowText[idx]);

        char* key = strstr(lowText, input);
        int32_t pos = key - lowText, len = strlen(input);
        if (key) {
            num++;
            printf("\033[0m%02d. \033[1;34m%s", num, content.book);
            printf("\033[0;34m %02d:%02d \033[0;90m", content.ch, content.ver);
            for (size_t idx = 0; idx < strlen(content.text); idx++) {
                if (idx == pos) printf("\033[1;32m");
                if (idx == (pos + len)) printf("\033[0;90m");
                printf("%c", content.text[idx]);
            }
            printf("\n");
        }
    }
    fclose(bFile);
    printf("\033[1;32mFound %d time(s)\n\033[0m", num);

    return 0;
}
