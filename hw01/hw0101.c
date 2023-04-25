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

#include "mystring.h"
#include "subaryaString.h"
sigjmp_buf env;
int wrongSunnyPT[7] = {0}, wrongOtherPT[7] = {0};
void recvSignal(int sig) {
    // printf("received signal %d !!!\n",sig);
    siglongjmp(env, 1);
}
void handle_alarm(int signal) {
    // printf("signal = %d\n", signal);
    siglongjmp(env, 2);
}
int score(const char *item, int strcmpNum) {
    if (strcmpNum == 0) {
        if (strcmp(item, "sunny") == 0) {  // sunny 2pts
            return 2;
        } else {  // other 1pt
            return 1;
        }
    }
    return 0;
}
void testFileInput(const char *srya, const char *stu) {
    // --- test file input ---
    FILE *fptr;
    fptr = fopen("ansstrchr.txt", "w");
    fprintf(fptr, "%s\n", srya);
    fclose(fptr);
    fptr = fopen("stustrchr.txt", "w");
    fprintf(fptr, "%s\n", stu);
    fclose(fptr);
    // --- end test file input ---
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
bool isSunnyWrong(void) {
    for (int i = 0; i < 7; ++i) {
        if (wrongSunnyPT[i] == 1) {
            return true;
        }
    }
    return false;
}
bool isOtherWrong(void) {
    for (int i = 0; i < 7; ++i) {
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
        printf("Sunny Case Wrong Function: ");
        for (int i = 0; i < 7; i++) {
            if (wrongSunnyPT[i] == 1) {
                printf("%d, ", i + 1);
            }
        }
        printf("\n");
    }
    if (isOtherWrong()) {
        printf("Other Case Wrong Function: ");
        for (int i = 0; i < 7; i++) {
            if (wrongOtherPT[i] == 1) {
                printf("%d, ", i + 1);
            }
        }
        printf("\n");
    }
}
int judgestrchr(const char *str, const char *bigstr, const char *name) {
    int pt = 0;
    // sunny
    if (strcmp(name, "sunny") == 0) {
        char *subaryastrchrSunnyResult;
        subaryastrchrSunnyResult = subaryastrchr(str, 'R');
        char *stustrchrSunnyResult;
        stustrchrSunnyResult = mystrchr(str, 'R');
        pt = score(name, judgeStringCmp(subaryastrchrSunnyResult, stustrchrSunnyResult));
        // printf("srya = %s\n", subaryastrchrSunnyResult);
        // printf("stu = %s\n", stustrchrSunnyResult);
        // printf("%d\n", pt);
        if (!pt) {
            ++wrongSunnyPT[0];
            return 0;
        } else {
            return pt - 1;
        }
    }
    // other
    else {
        char *subaryastrchrOtherResult;
        subaryastrchrOtherResult = subaryastrchr(bigstr, 'R');
        char *stustrchrOtherResult;
        stustrchrOtherResult = mystrchr(bigstr, 'R');
        pt = score(name, judgeStringCmp(subaryastrchrOtherResult, stustrchrOtherResult));
        // --- test file input ---
        // testFileInput(subaryastrchrOtherResult, stustrchrOtherResult);
        // --- end test file input ---
        // printf("%d\n", pt);
        if (!pt) {
            ++wrongOtherPT[0];
            return 0;
        } else {
            return pt;
        }
    }
}
int judgestrrchr(const char *str, const char *bigstr, const char *name) {
    int pt = 0;
    // sunny
    if (strcmp(name, "sunny") == 0) {
        char *subaryastrrchrSunnyResult;
        subaryastrrchrSunnyResult = subaryastrrchr(str, 'l');
        char *stustrrchrSunnyResult;
        stustrrchrSunnyResult = mystrrchr(str, 'l');
        pt = score(name, judgeStringCmp(subaryastrrchrSunnyResult, stustrrchrSunnyResult));
        // printf("srya = %s\n", subaryastrrchrSunnyResult);
        // printf("stu = %s\n", stustrrchrSunnyResult);
        // printf("%d\n", pt);
        if (!pt) {
            ++wrongSunnyPT[1];
            return 0;
        } else {
            return pt;
        }
    }
    // other
    else {
        char *subaryastrrchrOtherResult;
        subaryastrrchrOtherResult = subaryastrrchr(bigstr, 'l');
        char *stustrrchrOtherResult;
        stustrrchrOtherResult = mystrrchr(bigstr, 'l');
        pt = score(name, judgeStringCmp(subaryastrrchrOtherResult, stustrrchrOtherResult));
        // printf("srya = %s\n", subaryastrrchrOtherResult);
        // printf("stu = %s\n", stustrrchrOtherResult);
        // printf("%d\n", pt);
        // --- test file input ---
        // testFileInput(subaryastrrchrOtherResult, stustrrchrOtherResult);
        // --- end test file input ---
        // printf("%d\n", pt);
        if (!pt) {
            ++wrongOtherPT[1];
            return 0;
        } else {
            return pt;
        }
    }
}
int judgestrspn(const char *str, const char *bigstr, const char *name) {
    int pt = 0;
    // sunny
    if (strcmp(name, "sunny") == 0) {
        size_t subaryastrspnSunnyResult;
        subaryastrspnSunnyResult = subaryastrspn(str, "nm lateS");
        size_t stustrspnSunnyResult;
        stustrspnSunnyResult = mystrspn(str, "nm lateS");
        pt = score(name, judgeSizeCmp(subaryastrspnSunnyResult, stustrspnSunnyResult));
        // printf("srya = %lu\n", subaryastrspnSunnyResult);
        // printf("stu = %lu\n", stustrspnSunnyResult);
        // printf("%d\n", pt);
        if (!pt) {
            ++wrongSunnyPT[2];
            return 0;
        } else {
            return pt;
        }
    }
    // other
    else {
        size_t subaryastrspnOtherResult;
        subaryastrspnOtherResult = subaryastrspn(bigstr, "CRgZxFyoDYzV6KI,FsEY9h.L8FtGxhz9TvurG3ptftwEfh8QyXcrykT2CS1z0BWEmkX954MVXNXPlwXDRt1sCM48ov2weSmD3pkJdW8FYSHxnZX76EhzOA,usIfSfid1j4B 9kQ1GBcPqlRNrEL2mE");
        size_t stustrspnOtherResult;
        stustrspnOtherResult = mystrspn(bigstr, "CRgZxFyoDYzV6KI,FsEY9h.L8FtGxhz9TvurG3ptftwEfh8QyXcrykT2CS1z0BWEmkX954MVXNXPlwXDRt1sCM48ov2weSmD3pkJdW8FYSHxnZX76EhzOA,usIfSfid1j4B 9kQ1GBcPqlRNrEL2mE");
        pt = score(name, judgeSizeCmp(subaryastrspnOtherResult, stustrspnOtherResult));
        // printf("srya = %lu\n", subaryastrspnOtherResult);
        // printf("stu = %lu\n", stustrspnOtherResult);
        // printf("%d\n", pt);
        // --- test file input ---
        // FILE *fptr;
        // fptr = fopen("ansstrchr.txt", "w");
        // fprintf(fptr, "strchr: %lu\n", subaryastrspnOtherResult);
        // fclose(fptr);
        // fptr = fopen("stustrchr.txt", "w");
        // fprintf(fptr, "strchr: %lu\n", stustrspnOtherResult);
        // fclose(fptr);
        // --- end test file input ---
        // printf("%d\n", pt);
        if (!pt) {
            ++wrongOtherPT[2];
            return 0;
        } else {
            return pt;
        }
    }
}
int judgestrcspn(const char *str, const char *bigstr, const char *name) {
    int pt = 0;
    // sunny
    if (strcmp(name, "sunny") == 0) {
        size_t subaryastrcspnSunnyResult;
        subaryastrcspnSunnyResult = subaryastrcspn(str, "fu");
        size_t stustrcspnSunnyResult;
        stustrcspnSunnyResult = mystrcspn(str, "fu");
        pt = score(name, judgeSizeCmp(subaryastrcspnSunnyResult, stustrcspnSunnyResult));
        // printf("srya = %lu\n", subaryastrcspnSunnyResult);
        // printf("stu = %lu\n", stustrcspnSunnyResult);
        // printf("%d\n", pt);
        if (!pt) {
            ++wrongSunnyPT[3];
            return 0;
        } else {
            return pt;
        }
    }
    // other
    else {
        size_t subaryastrcspnOtherResult;
        subaryastrcspnOtherResult = subaryastrcspn(bigstr, "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbabbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");
        size_t stustrcspnOtherResult;
        stustrcspnOtherResult = mystrcspn(bigstr, "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbabbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");
        pt = score(name, judgeSizeCmp(subaryastrcspnOtherResult, stustrcspnOtherResult));
        // printf("srya = %lu\n", subaryastrcspnOtherResult);
        // printf("stu = %lu\n", stustrcspnOtherResult);
        // printf("%d\n", pt);
        // --- test file input ---
        // FILE *fptr;
        // fptr = fopen("ansstrchr.txt", "w");
        // fprintf(fptr, "strchr: %lu\n", subaryastrcspnOtherResult);
        // fclose(fptr);
        // fptr = fopen("stustrchr.txt", "w");
        // fprintf(fptr, "strchr: %lu\n", stustrcspnOtherResult);
        // fclose(fptr);
        // --- end test file input ---
        // printf("%d\n", pt);
        if (!pt) {
            ++wrongOtherPT[3];
            return 0;
        } else {
            return pt;
        }
    }
}
int judgestrpbrk(const char *str, const char *bigstr, const char *name) {
    int pt = 0;
    // sunny
    if (strcmp(name, "sunny") == 0) {
        char *subaryastrpbrkSunnyResult;
        subaryastrpbrkSunnyResult = subaryastrpbrk(str, "d f");
        char *stustrpbrkSunnyResult;
        stustrpbrkSunnyResult = mystrpbrk(str, "d f");
        pt = score(name, judgeStringCmp(subaryastrpbrkSunnyResult, stustrpbrkSunnyResult));
        // printf("srya = %s\n", subaryastrpbrkSunnyResult);
        // printf("stu = %s\n", stustrpbrkSunnyResult);
        // printf("%d\n", pt);
        if (!pt) {
            ++wrongSunnyPT[4];
            return 0;
        } else {
            return pt;
        }
    }
    // other
    else {
        char *subaryastrpbrkOtherResult;
        subaryastrpbrkOtherResult = subaryastrpbrk(bigstr, "bqbqbqbqbqbqbqbqbqbqbqbqbqbqbqbqbqbq");
        char *stustrpbrkOtherResult;
        stustrpbrkOtherResult = mystrpbrk(bigstr, "bqbqbqbqbqbqbqbqbqbqbqbqbqbqbqbqbqbq");
        pt = score(name, judgeStringCmp(subaryastrpbrkOtherResult, stustrpbrkOtherResult));
        // printf("srya = %s\n", subaryastrpbrkOtherResult);
        // printf("stu = %s\n", stustrpbrkOtherResult);
        // printf("%d\n", pt);
        // --- test file input ---
        // testFileInput(subaryastrpbrkOtherResult, stustrpbrkOtherResult);
        // --- end test file input ---
        // printf("%d\n", pt);
        if (!pt) {
            ++wrongOtherPT[4];
            return 0;
        } else {
            return pt;
        }
    }
}
int judgestrstr(const char *str, const char *bigstr, const char *name) {
    int pt = 0;
    // sunny
    if (strcmp(name, "sunny") == 0) {
        char *subaryastrstrSunnyResult;
        subaryastrstrSunnyResult = subaryastrstr(str, "en");
        char *stustrstrSunnyResult;
        stustrstrSunnyResult = mystrstr(str, "en");
        pt = score(name, judgeStringCmp(subaryastrstrSunnyResult, stustrstrSunnyResult));
        // printf("srya = %s\n", subaryastrstrSunnyResult);
        // printf("stu = %s\n", stustrstrSunnyResult);
        // printf("%d\n", pt);
        if (!pt) {
            ++wrongSunnyPT[5];
            return 0;
        } else {
            return pt;
        }
    }
    // other
    else {
        char *subaryastrstrOtherResult;
        subaryastrstrOtherResult = subaryastrstr(bigstr, "rab4eozj97dRvXZeKWY4OfiG4lYYL7DoVHLIkJzdAIHLOZXupim2SaN9e0ScP2AL1,oX S9UMTFzKg476Y7qKfPnizxTluCy6xidA3LCBa2rp3XFRYrk,LmeRkegyz0ohdC hSb8dXAg8MZBUBo3g");
        char *stustrstrOtherResult;
        stustrstrOtherResult = mystrstr(bigstr, "rab4eozj97dRvXZeKWY4OfiG4lYYL7DoVHLIkJzdAIHLOZXupim2SaN9e0ScP2AL1,oX S9UMTFzKg476Y7qKfPnizxTluCy6xidA3LCBa2rp3XFRYrk,LmeRkegyz0ohdC hSb8dXAg8MZBUBo3g");
        // testFileInput(subaryastrstrOtherResult, stustrstrOtherResult);
        pt = score(name, judgeStringCmp(subaryastrstrOtherResult, stustrstrOtherResult));
        // printf("srya = %s\n", subaryastrstrOtherResult);
        // printf("stu = %s\n", stustrstrOtherResult);
        // printf("%d\n", pt);
        // --- test file input ---
        // testFileInput(subaryastrstrOtherResult, stustrstrOtherResult);
        // --- end test file input ---
        // printf("%d\n", pt);
        if (!pt) {
            ++wrongOtherPT[5];
            return 0;
        } else {
            return pt;
        }
    }
}
int judgestrtok(char *str, char *bigstr, const char *name) {
    int pt = 0;
    // sunny
    if (strcmp(name, "sunny") == 0) {
        char *copystr = strdup(str);
        FILE *fptr;
        fptr = fopen("studentSunny.txt", "w");
        char *stustrtokSunnyToken = mystrtok(copystr, ",. ");
        while (stustrtokSunnyToken != NULL) {
            fprintf(fptr, "strtok: %s\n", stustrtokSunnyToken);
            stustrtokSunnyToken = mystrtok(NULL, ",. ");
        }
        fclose(fptr);
        free(copystr);
        FILE *file1 = fopen("subaryaStrtokAnsSunny.txt", "r");  // subarya ans file
        FILE *file2 = fopen("studentSunny.txt", "r");           // stu ans file
        if (file2 == NULL) {
            fclose(file1);
            fclose(file2);
            return pt;
        }
        if (fileCmp(file1, file2)) {
            // printf("Sunny good\n");
            pt = 2;
            fclose(file1);
            fclose(file2);
            return pt;
        }
        // printf("Sunny huh?\n");
        fclose(file1);
        fclose(file2);
        ++wrongSunnyPT[6];
        return 0;

    }
    // other
    else {
        char *copystr = strdup(bigstr);
        FILE *fptr;
        fptr = fopen("studentOther.txt", "w");
        char *stustrtokOtherToken = mystrtok(copystr, ",. ");
        while (stustrtokOtherToken != NULL) {
            fprintf(fptr, "strtok: %s\n", stustrtokOtherToken);
            stustrtokOtherToken = mystrtok(NULL, ",. ");
        }
        fclose(fptr);
        free(copystr);
        FILE *file1 = fopen("subaryaStrtokAnsOther.txt", "r");  // subarya ans file
        FILE *file2 = fopen("studentOther.txt", "r");           // stu ans file
        if (file2 == NULL) {
            fclose(file1);
            fclose(file2);
            return pt;
        }
        if (fileCmp(file1, file2)) {
            // printf("Other good\n");
            pt = 1;
            fclose(file1);
            fclose(file2);
            return pt;
        }
        // printf("Other huh?\n");
        fclose(file1);
        fclose(file2);
        ++wrongOtherPT[6];
        return 0;
    }
}
int main() {
    int r;  // setjmp val
    // sunny case (13pts)
    char str1[] = "SubaRya TA is handsome";
    char str2[] = "Smallten and Mafumafu are cute";
    char str3[] = "jw910731, LoveSnowEx, Smallten, SubaRya, NTNU Students, also Computer Programming II TA, work hard";
    // other case (7pts)
    char *bigstr1;
    int size1;
    struct stat s1;
    const char *file_name1 = "./str1.txt";
    int fd1 = open(file_name1, O_RDONLY);
    int status1 = fstat(fd1, &s1);
    size1 = s1.st_size;
    bigstr1 = (char *)mmap(0, size1 + 1, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd1, 0);
    bigstr1[size1] = '\0';

    char *bigstr2;
    int size2;
    struct stat s2;
    const char *file_name2 = "./str2.txt";
    int fd2 = open(file_name2, O_RDONLY);
    int status2 = fstat(fd2, &s2);
    size2 = s2.st_size;
    bigstr2 = (char *)mmap(0, size2 + 1, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd2, 0);
    bigstr2[size2] = '\0';

    char *bigstr3;
    int size3;
    struct stat s3;
    const char *file_name3 = "./str3.txt";
    int fd3 = open(file_name3, O_RDONLY);
    int status3 = fstat(fd3, &s3);
    size3 = s3.st_size;
    bigstr3 = (char *)mmap(0, size3 + 1, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd3, 0);
    bigstr3[size3] = '\0';

    // --------------------
    // judge sunnycase
    // --------------------

    int totalPoint = 0, tmpPT = 0;

    // judgestrchr sunny case
    r = sigsetjmp(env, 1);
    if (r == 0) {
        signal(SIGSEGV, recvSignal);
        signal(SIGALRM, handle_alarm);
        alarm(10);
        tmpPT = 0;
        tmpPT += judgestrchr(str1, bigstr1, "sunny");
        totalPoint += tmpPT;
        alarm(0);
    } else if (r == 1) {
        printf("You segmentation fault. Go back to checkpoint!\n");
        wrongSunnyPT[0] = 1;
    } else {
        printf("You died. Go back to checkpoint!\n");
        wrongSunnyPT[0] = 1;
    }

    // judgestrrchr sunny case
    r = sigsetjmp(env, 1);
    if (r == 0) {
        signal(SIGSEGV, recvSignal);
        signal(SIGALRM, handle_alarm);
        alarm(10);
        tmpPT = 0;
        tmpPT = judgestrrchr(str2, bigstr2, "sunny");
        totalPoint += tmpPT;
        alarm(0);
    } else if (r == 1) {
        printf("You segmentation fault. Go back to checkpoint!\n");
        wrongSunnyPT[1] = 1;
    } else {
        printf("You died. Go back to checkpoint!\n");
        wrongSunnyPT[1] = 1;
    }

    // judgestrspn sunny case
    r = sigsetjmp(env, 1);
    if (r == 0) {
        signal(SIGSEGV, recvSignal);
        signal(SIGALRM, handle_alarm);
        alarm(10);
        tmpPT = 0;
        tmpPT = judgestrspn(str2, bigstr2, "sunny");
        totalPoint += tmpPT;
        alarm(0);
    } else if (r == 1) {
        printf("You segmentation fault. Go back to checkpoint!\n");
        wrongSunnyPT[2] = 1;
    } else {
        printf("You died. Go back to checkpoint!\n");
        wrongSunnyPT[2] = 1;
    }

    // judgestrcspn sunny case
    r = sigsetjmp(env, 1);
    if (r == 0) {
        signal(SIGSEGV, recvSignal);
        signal(SIGALRM, handle_alarm);
        alarm(10);
        tmpPT = 0;
        tmpPT = judgestrcspn(str2, bigstr2, "sunny");
        totalPoint += tmpPT;
        alarm(0);
    } else if (r == 1) {
        printf("You segmentation fault. Go back to checkpoint!\n");
        wrongSunnyPT[3] = 1;
    } else {
        printf("You died. Go back to checkpoint!\n");
        wrongSunnyPT[3] = 1;
    }

    // judgestrpbrk sunny case
    r = sigsetjmp(env, 1);
    if (r == 0) {
        signal(SIGSEGV, recvSignal);
        signal(SIGALRM, handle_alarm);
        alarm(10);
        tmpPT = 0;
        tmpPT = judgestrpbrk(str2, bigstr2, "sunny");
        totalPoint += tmpPT;
        alarm(0);
    } else if (r == 1) {
        printf("You segmentation fault. Go back to checkpoint!\n");
        wrongSunnyPT[4] = 1;
    } else {
        printf("You died. Go back to checkpoint!\n");
        wrongSunnyPT[4] = 1;
    }

    // judgestrstr sunny case
    r = sigsetjmp(env, 1);
    if (r == 0) {
        signal(SIGSEGV, recvSignal);
        signal(SIGALRM, handle_alarm);
        alarm(10);
        tmpPT = 0;
        tmpPT += judgestrstr(str3, bigstr3, "sunny");
        totalPoint += tmpPT;
        alarm(0);
    } else if (r == 1) {
        printf("You segmentation fault. Go back to checkpoint!\n");
        wrongSunnyPT[5] = 1;
    } else {
        printf("You died. Go back to checkpoint!\n");
        wrongSunnyPT[5] = 1;
    }

    // judgestrtok sunny case
    r = sigsetjmp(env, 1);
    if (r == 0) {
        signal(SIGSEGV, recvSignal);
        signal(SIGALRM, handle_alarm);
        alarm(10);
        tmpPT = 0;
        tmpPT = judgestrtok(str3, bigstr3, "sunny");
        totalPoint += tmpPT;
        alarm(0);
    } else if (r == 1) {
        printf("You segmentation fault. Go back to checkpoint!\n");
        wrongSunnyPT[6] = 1;
    } else {
        printf("You died. Go back to checkpoint!\n");
        wrongSunnyPT[6] = 1;
    }

    // printf("current sunny case PT = %d\n", totalPoint);

    // --------------------
    // judge othercase
    // --------------------

    // judgestrchr other case
    r = sigsetjmp(env, 1);
    if (r == 0) {
        signal(SIGSEGV, recvSignal);
        signal(SIGALRM, handle_alarm);
        alarm(10);
        tmpPT = 0;
        tmpPT = judgestrchr(str1, bigstr1, "other");
        totalPoint += tmpPT;
        alarm(0);
    } else if (r == 1) {
        printf("You segmentation fault. Go back to checkpoint!\n");
        wrongOtherPT[0] = 1;
    } else {
        printf("You loop. Go back to checkpoint!\n");
        wrongOtherPT[0] = 1;
    }

    // judgestrrchr other case
    r = sigsetjmp(env, 1);
    if (r == 0) {
        signal(SIGSEGV, recvSignal);
        signal(SIGALRM, handle_alarm);
        alarm(10);
        tmpPT = 0;
        tmpPT = judgestrrchr(str2, bigstr2, "other");
        totalPoint += tmpPT;
        alarm(0);
    } else if (r == 1) {
        printf("You segmentation fault. Go back to checkpoint!\n");
        wrongOtherPT[1] = 1;
    } else {
        printf("You loop. Go back to checkpoint!\n");
        wrongOtherPT[1] = 1;
    }

    // judgestrspn other case
    r = sigsetjmp(env, 1);
    if (r == 0) {
        signal(SIGSEGV, recvSignal);
        signal(SIGALRM, handle_alarm);
        alarm(10);
        tmpPT = 0;
        tmpPT = judgestrspn(str2, bigstr2, "other");
        totalPoint += tmpPT;
        alarm(0);
    } else if (r == 1) {
        printf("You segmentation fault. Go back to checkpoint!\n");
        wrongOtherPT[2] = 1;
    } else {
        printf("You loop. Go back to checkpoint!\n");
        wrongOtherPT[2] = 1;
    }

    // judgestrcspn other case
    r = sigsetjmp(env, 1);
    if (r == 0) {
        signal(SIGSEGV, recvSignal);
        signal(SIGALRM, handle_alarm);
        alarm(10);
        tmpPT = 0;
        tmpPT = judgestrcspn(str2, bigstr2, "other");
        totalPoint += tmpPT;
        alarm(0);
    } else if (r == 1) {
        printf("You segmentation fault. Go back to checkpoint!\n");
        wrongOtherPT[3] = 1;
    } else {
        printf("You loop. Go back to checkpoint!\n");
        wrongOtherPT[3] = 1;
    }

    // judgestrpbrk other case
    r = sigsetjmp(env, 1);
    if (r == 0) {
        signal(SIGSEGV, recvSignal);
        signal(SIGALRM, handle_alarm);
        alarm(10);
        tmpPT = 0;
        tmpPT = judgestrpbrk(str2, bigstr2, "other");
        totalPoint += tmpPT;
        alarm(0);
    } else if (r == 1) {
        printf("You segmentation fault. Go back to checkpoint!\n");
        wrongOtherPT[4] = 1;
    } else {
        printf("You loop. Go back to checkpoint!\n");
        wrongOtherPT[4] = 1;
    }

    // judgestrstr other case
    r = sigsetjmp(env, 1);
    if (r == 0) {
        signal(SIGSEGV, recvSignal);
        signal(SIGALRM, handle_alarm);
        alarm(10);
        tmpPT = 0;
        tmpPT = judgestrstr(str3, bigstr3, "other");
        totalPoint += tmpPT;
        alarm(0);
    } else if (r == 1) {
        printf("You segmentation fault. Go back to checkpoint!\n");
        wrongOtherPT[5] = 1;
    } else {
        printf("You died. Go back to checkpoint!\n");
        wrongOtherPT[5] = 1;
    }

    // judgestrtok other case
    r = sigsetjmp(env, 1);
    if (r == 0) {
        signal(SIGSEGV, recvSignal);
        signal(SIGALRM, handle_alarm);
        alarm(10);
        tmpPT = 0;
        tmpPT = judgestrtok(str3, bigstr3, "other");
        totalPoint += tmpPT;
        alarm(0);
    } else if (r == 1) {
        printf("You segmentation fault. Go back to checkpoint!\n");
        wrongOtherPT[6] = 1;
    } else {
        printf("You loop. Go back to checkpoint!\n");
        wrongOtherPT[6] = 1;
    }

    printf("total PT = %d\n", totalPoint);
    printResult();
    munmap(bigstr1, size1);
    munmap(bigstr2, size2);
    munmap(bigstr3, size3);
    close(fd1);
    close(fd2);
    close(fd3);
    return 0;
}
