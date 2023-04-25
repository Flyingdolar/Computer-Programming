#include <fcntl.h>
#include <setjmp.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "insert.h"
sigjmp_buf env;
int wrongSunnyPT[7] = {0}, wrongOtherPT[5] = {0};
void recvSignal(int sig) {
    // printf("received signal %d !!!\n",sig);
    siglongjmp(env, 1);
}
void handle_alarm(int signal) {
    // printf("signal = %d\n", signal);
    siglongjmp(env, 2);
}

void handle_abort(int signal) {
    siglongjmp(env, 3);
}

bool fileCmp(FILE *fPtr1, FILE *fPtr2) {
    char ch1, ch2;
    do {
        // Input character from both files
        ch1 = fgetc(fPtr1);
        ch2 = fgetc(fPtr2);
        // If characters are not same then return -1
        if (ch1 != ch2) {
            return false;
        }
    } while (ch1 != EOF && ch2 != EOF);
    /* If both files have reached end */
    if (ch1 == EOF && ch2 == EOF) {
        return true;
    } else {
        return false;
    }
}
char *stringCat(char *str1, char *str2, char *str3) {
    char *newStr;
    newStr = malloc(strlen(str1) + strlen(str2) + strlen(str3) + 1);
    newStr[0] = '\0';  // ensures the memory is an empty string
    strcat(newStr, str1);
    strcat(newStr, str2);
    strcat(newStr, str3);
    return newStr;
}
bool isSunnyWrong(void) {
    for (int i = 0; i < 6; ++i) {
        if (wrongSunnyPT[i] == 1) {
            return true;
        }
    }
    return false;
}
bool isOtherWrong(void) {
    for (int i = 0; i < 4; ++i) {
        if (wrongOtherPT[i] == 1) {
            return true;
        }
    }
    return false;
}
void printResult(void) {
    if (!isSunnyWrong() && !isOtherWrong()) {
        printf("Get 20 pts, all correct!!!\n");
        return;
    }
    if (isSunnyWrong()) {
        printf("Wrong Sunny Case: ");
        for (int i = 0; i < 6; i++) {
            if (wrongSunnyPT[i] == 1) {
                printf("%d, ", i + 1);
            }
        }
        printf("\n");
    }
    if (isOtherWrong()) {
        printf("Wrong Other Case: ");
        for (int i = 0; i < 4; i++) {
            if (wrongOtherPT[i] == 1) {
                printf("%d, ", i + 1);
            }
        }
        printf("\n");
    }
}
int judgeStringInsert(char *str1, char *str2, int pos, const char *targetFile, int qNum) {
    // printf("in function\n");
    // printf("pos = %d\n", pos);
    char *result = NULL;
    int pt = 0, ret;
    ret = strinsert(&result, str1, pos, str2);
    if (ret == -1) {
        FILE *fptr;
        fptr = fopen("./test/stuAns.txt", "w");
        fprintf(fptr, "%s", "-1");
        // free(result);
        fclose(fptr);
    } else {
        if (result) {
            FILE *fptr;
            fptr = fopen("./test/stuAns.txt", "w");
            fprintf(fptr, "%s", result);
            free(result);
            fclose(fptr);
        }
    }

    FILE *file1 = fopen(targetFile, "r");           // subarya ans file
    FILE *file2 = fopen("./test/stuAns.txt", "r");  // stu ans file
    if (file2 == NULL) {
        fclose(file1);
        fclose(file2);
        return pt;
    }
    if (fileCmp(file1, file2)) {
        // printf("good\n");
        pt = 2;
        fclose(file1);
        fclose(file2);
        return pt;
    }
    // printf("huh?\n");
    fclose(file1);
    fclose(file2);
    // if answer is wrong, then check it is sunny case or not
    if (targetFile[9] <= '6') {
        ++wrongSunnyPT[qNum];
    } else {
        ++wrongOtherPT[qNum];
    }
    return 0;
}
// add: siglongjmp for SIGSEGV and SIGALRM
// add: use mmap and munmap to read file

int main() {
    // test sunny case
    int r, tmpPT, totalPoint = 0, sunnyPos[6] = {99, 41, 1000, 78, 22, 39}, otherPos[4] = {2075752, 14199144, 6903595, 18798873};
    for (int i = 1; i <= 6; ++i) {
        char *str1, *str2;
        int size1, size2;
        char idx[2];
        idx[1] = '\0';
        idx[0] = i + '0';
        const char *file_name1 = stringCat("./test/sunnyCase/sunnyCase", idx, "str1.txt");
        const char *file_name2 = stringCat("./test/sunnyCase/sunnyCase", idx, "str2.txt");
        const char *ans_file_name = stringCat(".test/ans/ans", idx, ".txt");
        // printf("%s\n", file_name1);
        // printf("%s\n", file_name2);
        // file 1
        int fd1 = open(file_name1, O_RDONLY);
        if (fd1 == -1) {
            perror("open()");
            return 1;
        }
        struct stat s1;
        int status1 = fstat(fd1, &s1);
        size1 = s1.st_size;
        str1 = (char *)mmap(0, size1, PROT_READ, MAP_PRIVATE, fd1, 0);

        // file 2
        int fd2 = open(file_name2, O_RDONLY);
        if (fd2 == -1) {
            perror("open()");
            return 1;
        }
        struct stat s2;
        int status2 = fstat(fd2, &s2);
        size2 = s2.st_size;
        str2 = (char *)mmap(0, size2, PROT_READ, MAP_PRIVATE, fd2, 0);

        // ------------ do somthing ------------
        // printf("%s\n", str1);
        // printf("%s\n", str2);
        r = sigsetjmp(env, 1);
        if (r == 0) {
            signal(SIGSEGV, recvSignal);
            signal(SIGALRM, handle_alarm);
            signal(SIGABRT, handle_abort);
            alarm(10);
            tmpPT = 0;
            tmpPT += judgeStringInsert(str1, str2, sunnyPos[i - 1], ans_file_name, i - 1);
            totalPoint += tmpPT;
            // printf("totalPT = %d\n", totalPoint);
            alarm(0);
        } else if (r == 1) {
            printf("You segmentation fault. Go back to checkpoint!\n");
            wrongSunnyPT[i - 1] = 1;
        } else if (r == 2) {
            printf("You died. Go back to checkpoint!\n");
            wrongSunnyPT[i - 1] = 1;
        } else {
            printf("Invalid pointer!\n");
            wrongSunnyPT[i - 1] = 1;
        }
        // ------------ end something ------------
        close(fd1);
        close(fd2);
        munmap(str1, size1);
        munmap(str2, size2);
    }

    // test other case
    for (int i = 1; i <= 4; ++i) {
        char *str1, *str2;
        size_t size1, size2;
        char idxT[2], idxA[2];
        idxT[1] = '\0';
        idxA[1] = '\0';
        idxT[0] = i + '0';
        if (i < 4) {
            idxA[0] = i + '6';
        } else {
            idxA[0] = '0';
        }
        const char *file_name1 = stringCat("./test/otherCase/otherCase", idxT, "str1.txt");
        const char *file_name2 = stringCat("./test/otherCase/otherCase", idxT, "str2.txt");
        char *ans_file_name;
        if (i < 4) {
            ans_file_name = stringCat(".test/ans/ans", idxA, ".txt");
        } else {
            ans_file_name = stringCat(".test/ans/ans", "10", ".txt");
        }
        // printf("%s\n", file_name1);
        // printf("%s\n", file_name2);
        // printf("%s\n", ans_file_name);
        // file 1
        int fd1 = open(file_name1, O_RDONLY);
        if (fd1 == -1) {
            perror("open()");
            return 1;
        }
        struct stat s1;
        int status1 = fstat(fd1, &s1);
        size1 = s1.st_size;
        str1 = (char *)mmap(0, size1, PROT_READ, MAP_PRIVATE, fd1, 0);
        close(fd1);
        char *buf1 = malloc(size1 + 1);
        memcpy(buf1, str1, size1);
        buf1[size1] = '\0';
        size_t len = strlen(buf1);
        // printf("The length of the string is %zu\n", len);

        // file 2
        int fd2 = open(file_name2, O_RDONLY);
        if (fd2 == -1) {
            perror("open()");
            return 1;
        }
        struct stat s2;
        int status2 = fstat(fd2, &s2);
        size2 = s2.st_size;
        str2 = (char *)mmap(0, size2, PROT_READ, MAP_PRIVATE, fd2, 0);
        char *buf2 = malloc(size1 + 1);
        memcpy(buf2, str1, size1);
        buf2[size1] = '\0';
        close(fd2);
        // ------------ do somthing ------------
        r = sigsetjmp(env, 1);
        if (r == 0) {
            signal(SIGSEGV, recvSignal);
            signal(SIGALRM, handle_alarm);
            signal(SIGABRT, handle_abort);
            alarm(10);
            tmpPT = 0;
            tmpPT += judgeStringInsert(str1, str2, otherPos[i - 1], ans_file_name, i - 1);
            totalPoint += tmpPT;
            // printf("totalPT = %d\n", totalPoint);
            alarm(0);
        } else if (r == 1) {
            printf("You segmentation fault. Go back to checkpoint!\n");
            wrongOtherPT[i - 1] = 1;
        } else if (r == 2) {
            printf("You died. Go back to checkpoint!\n");
            wrongOtherPT[i - 1] = 1;
        } else {
            printf("Invalid pointer!\n");
            wrongOtherPT[i - 1] = 1;
        }
        // ------------ end something ------------
        free(buf1);
        free(buf2);
        close(fd1);
        close(fd2);
        munmap(str1, size1);
        munmap(str2, size2);
    }
    printf("totalPT = %d\n", totalPoint);
    printResult();
    return 0;
}
