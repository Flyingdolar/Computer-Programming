#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define CMD_LEN 1000
#define PR_GREEN(...) \
    printf("\033[0;32m"), printf(__VA_ARGS__), printf("\033[0m")
#define PR_RED(...) \
    printf("\033[0;31m"), printf(__VA_ARGS__), printf("\033[0m")
#define PR_YELLOW(...) \
    printf("\033[0;33m"), printf(__VA_ARGS__), printf("\033[0m")
#define PR_GRAY(...) \
    printf("\033[0;37m"), printf(__VA_ARGS__), printf("\033[0m")

#define PRINTE(...) printf("\033[0;31mE: "), printf(__VA_ARGS__), printf("\033[0m\n")

int getRandom(int min, int max) {
    static int init = 0;

    if (!init)
        srand(time(NULL)), init = 1;
    return min + rand() % (max - min + 1);
}

int wNum(char letter) {
    return letter - 'a';
}

void setZero(int *arr, int size) {
    for (int i = 0; i < size; i++) arr[i] = 0;
    return;
}

void setDict(char *dictFile, char *rule) {
    char cmd[CMD_LEN] = "sed -n ";
    strcat(cmd, rule), strcat(cmd, " /usr/share/dict/american-english");
    strcat(cmd, " | tr '[:upper:]' '[:lower:]'");
    strcat(cmd, " >> "), strcat(cmd, dictFile);
    system(cmd);
    return;
}

char *getWord(char *dictFile, uint32_t index) {
    FILE *fp = fopen(dictFile, "r");
    char *word = malloc(100);
    for (int i = 0; i < index; i++) {
        if (fp == NULL) rewind(fp);
        fgets(word, 100, fp);
    }
    fclose(fp);
    return word;
}

int checkWord(char *gWord, char *dictFile) {
    FILE *fp = fopen(dictFile, "r");
    char *qWord = malloc(100);
    while (fgets(qWord, 100, fp) != NULL) {
        qWord[strlen(qWord) - 1] = '\0';
        if (strcmp(gWord, qWord) == 0) {
            fclose(fp);
            return 0;
        }
    }
    fclose(fp);
    PR_RED("This is not a word in the dictionary, try again.\n");
    return -1;
}

int main(void) {
    char rule[CMD_LEN] = "'/^[a-zA-Z]\\{5\\}$/p'";
    char fileName[CMD_LEN] = "tmp.txt";
    char *qWord, gWord[100];
    int wCount[26], wAppear[26];

    setDict(fileName, rule);
    qWord = getWord(fileName, getRandom(1, 1000));
    printf("Answer: %s\n", qWord);

    for (size_t idx = 0; idx < 6; idx++) {
        printf("Guess %lu: ", idx + 1);
        scanf("%s", gWord), setZero(wCount, 26);
        if (checkWord(gWord, fileName) == -1) {
            --idx;
            continue;
        }
        for (size_t qdx = 0; qdx < strlen(qWord); qdx++)
            ++wCount[wNum(qWord[qdx])], wAppear[wNum(qWord[qdx])] = 1;
        for (size_t gdx = 0; gdx < strlen(gWord); gdx++) {
            if (strncmp(&gWord[gdx], &qWord[gdx], 1) == 0)
                wCount[wNum(gWord[gdx])]--;
        }
        for (size_t gdx = 0; gdx < strlen(gWord); gdx++) {
            if (strncmp(&gWord[gdx], &qWord[gdx], 1) == 0)
                PR_GREEN("%c", gWord[gdx]);
            else if (wCount[wNum(gWord[gdx])] > 0) {
                PR_YELLOW("%c", gWord[gdx]);
                wCount[wNum(gWord[gdx])]--;
            } else if (wAppear[wNum(gWord[gdx])] == 1)
                PR_GRAY("%c", gWord[gdx]);
            else
                printf("%c", gWord[gdx]);
        }
        printf("\n");
        if (strncmp(gWord, qWord, 5) == 0) {
            PR_GREEN("\nYou Got it! Congratulations!\n");
            break;
        }
    }
    if (strncmp(gWord, qWord, 5) != 0)
        PR_RED("You lose! The answer is %s\n", qWord);
    system("rm tmp.txt");
    return 0;
}
