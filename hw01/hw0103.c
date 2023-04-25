#define _DEFAULT_SOURCE
#include <setjmp.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "abacus.h"

sigjmp_buf env;

void recvSig(int sig) {
    siglongjmp(env, 1);
}

#define TEST_TYPE_DEFAULT 0
#define TEST_TYPE_ERROR 1

char *BIG_INTS[] = {
    "807281148313186643362940189859389177589850396574998165232211393001672158578366633642255789196790107014333076051067303297428783684810840385067643923215775198845132197416135009973360680631649328230204669830239913826501181837278414867203800064708949607419761",
    "190813186266910996217745958160814077511953394000730273224507252669826411831804757502365561180544071919052835711172052661954746196665362114544449869630518166420383967617147435711079269187400929038493035067928545142526991711807751922936636391262840759728877",
    "6945230966695569107534911460562807754239436008875320391542011723732438459170290131696541956710981518659792458455980291002466431014578882689832361076243701198851606834542376714600949880610486332510520515886754032840953811213680438454391435253889566398085709"};

int errorf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    fprintf(stderr, fmt, args);
    va_end(args);
    return 0;
}

int32_t testInit() {
    sAbacus *abacus = abacus_init();
    if (abacus == NULL) {
        errorf("Error: abacus_init() returned NULL\n");
        return -1;
    }

    if (abacus_print(*abacus) != 0) {
        errorf("Error: Print operation failed\n");
        return -1;
    }

    abacus_free(abacus);
    return 0;
}

int32_t testSet(char *s) {
    sAbacus *abacus = abacus_init();
    if (abacus == NULL) {
        errorf("Error: abacus_init() returned NULL\n");
        return -1;
    }

    if (abacus_set(abacus, s) != 0) {
        errorf("Error: Set operation failed\n");
        return -1;
    }

    if (abacus_print(*abacus) != 0) {
        errorf("Error: Print operation failed\n");
        return -1;
    }

    abacus_free(abacus);
    return 0;
}

int32_t testAdd(char *s1, char *s2) {
    sAbacus *abacus = abacus_init();
    sAbacus *abacus2 = abacus_init();
    sAbacus *abacus3 = abacus_init();
    if (abacus == NULL || abacus2 == NULL || abacus3 == NULL) {
        errorf("Error: abacus_init() returned NULL\n");
        return -1;
    }

    if (abacus_set(abacus, s1) != 0) {
        errorf("Error: Set operation failed\n");
        return -1;
    }
    if (abacus_set(abacus2, s2) != 0) {
        errorf("Error: Set operation failed\n");
        return -1;
    }

    if (abacus_add(abacus3, *abacus, *abacus2) != 0) {
        errorf("Error: Add operation failed\n");
        return -1;
    }

    if (abacus_print(*abacus3) != 0) {
        errorf("Error: Print operation failed\n");
        return -1;
    }

    abacus_free(abacus);
    abacus_free(abacus2);
    abacus_free(abacus3);
    return 0;
}

int32_t testDel(char *s1, char *s2) {
    sAbacus *abacus = abacus_init();
    sAbacus *abacus2 = abacus_init();
    sAbacus *abacus3 = abacus_init();
    if (abacus == NULL || abacus2 == NULL || abacus3 == NULL) {
        errorf("Error: abacus_init() returned NULL\n");
        return -1;
    }

    if (abacus_set(abacus, s1) != 0) {
        errorf("Error: Set operation failed\n");
        return -1;
    }
    if (abacus_set(abacus2, s2) != 0) {
        errorf("Error: Set operation failed\n");
        return -1;
    }

    if (abacus_del(abacus3, *abacus, *abacus2) != 0) {
        errorf("Error: Del operation failed\n");
        return -1;
    }

    if (abacus_print(*abacus3) != 0) {
        errorf("Error: Print operation failed\n");
        return -1;
    }

    abacus_free(abacus);
    abacus_free(abacus2);
    abacus_free(abacus3);
    return 0;
}

int32_t testErrorSet() {
    printf("Errorcase Set:\n");
    sAbacus *abacus = abacus_init();
    if (abacus == NULL) {
        errorf("Error: abacus_init() returned NULL\n");
        return -1;
    }

    if (abacus_set(NULL, "1234") != -1) {
        errorf("Error: Set operation should fail\n");
        goto FREE;
    }

    if (abacus_set(abacus, NULL) != -1) {
        errorf("Error: Set operation should fail\n");
        goto FREE;
    }

    if (abacus_set(abacus, "") != -1) {
        errorf("Error: Set operation should fail\n");
        goto FREE;
    }

    if (abacus_set(abacus, BIG_INTS[2]) != -1) {
        errorf("Error: Set operation should fail\n");
        goto FREE;
    }

    if (abacus_set(abacus, "1234a") != -1) {
        errorf("Error: Set operation should fail\n");
        goto FREE;
    }

    if (abacus_set(abacus, "1234.") != -1) {
        errorf("Error: Set operation should fail\n");
        goto FREE;
    }

    abacus_free(abacus);
    return 0;
FREE:
    abacus_free(abacus);
    return -1;
}

int32_t testErrorAdd() {
    printf("Errorcase Add:\n");
    sAbacus *abacus = abacus_init();
    sAbacus *abacus2 = abacus_init();
    sAbacus *abacus3 = abacus_init();
    if (abacus == NULL || abacus2 == NULL || abacus3 == NULL) {
        errorf("Error: abacus_init() returned NULL\n");
        return -1;
    }

    if (abacus_set(abacus, BIG_INTS[0]) != 0) {
        errorf("Error: Set operation failed\n");
        goto FREE;
    }

    if (abacus_set(abacus2, BIG_INTS[1]) != 0) {
        errorf("Error: Set operation failed\n");
        goto FREE;
    }

    if (abacus_add(NULL, *abacus, *abacus2) != -1) {
        errorf("Error: Add operation should fail\n");
        goto FREE;
    }

    if (abacus_add(abacus3, *abacus, *abacus) != -1) {
        errorf("Error: Add operation should fail\n");
        goto FREE;
    }

    abacus_free(abacus);
    abacus_free(abacus2);
    abacus_free(abacus3);
    return 0;
FREE:
    abacus_free(abacus);
    abacus_free(abacus2);
    abacus_free(abacus3);
    return -1;
}

int32_t testErrorDel() {
    printf("Errorcase Del:\n");
    sAbacus *abacus = abacus_init();
    sAbacus *abacus2 = abacus_init();
    sAbacus *abacus3 = abacus_init();
    if (abacus == NULL || abacus2 == NULL || abacus3 == NULL) {
        errorf("Error: abacus_init() returned NULL\n");
        return -1;
    }

    if (abacus_set(abacus, BIG_INTS[0]) != 0) {
        errorf("Error: Set operation failed\n");
        goto FREE;
    }

    if (abacus_set(abacus2, BIG_INTS[1]) != 0) {
        errorf("Error: Set operation failed\n");
        goto FREE;
    }

    if (abacus_del(NULL, *abacus, *abacus2) != -1) {
        errorf("Error: Del operation should fail\n");
        goto FREE;
    }

    if (abacus_del(abacus3, *abacus2, *abacus) != -1) {
        errorf("Error: Del operation should fail\n");
        goto FREE;
    }

    abacus_free(abacus);
    abacus_free(abacus2);
    abacus_free(abacus3);
    return 0;
FREE:
    abacus_free(abacus);
    abacus_free(abacus2);
    abacus_free(abacus3);
    return -1;
}

int32_t testcase1() {
    printf("Initialize an abacus\n");
    return testInit();
}

int32_t testcase2() {
    printf("Set an abacus to \"1234\"\n");
    return testSet("1234");
}

int32_t testcase3() {
    printf("Set an abacus to \"%s\"\n", BIG_INTS[0]);
    return testSet(BIG_INTS[0]);
}

int32_t testcase4() {
    printf("Add \"1234\" and \"5678\"\n");
    return testAdd("1234", "5678");
}

int32_t testcase5() {
    printf("Add \"5678\" and \"1234\"\n");
    return testAdd("5678", "1234");
}

int32_t testcase6() {
    printf("Add \"%s\" and \"%s\"\n", BIG_INTS[0], BIG_INTS[1]);
    return testAdd(BIG_INTS[0], BIG_INTS[1]);
}

int32_t testcase7() {
    printf("Subtract \"1234\" from \"5678\"\n");
    return testDel("5678", "1234");
}

int32_t testcase8() {
    printf("Subtract \"5678\" from \"9876\"\n");
    return testDel("9876", "5678");
}

int32_t testcase9() {
    printf("Subtract \"%s\" from \"%s\"\n", BIG_INTS[1], BIG_INTS[0]);
    return testDel(BIG_INTS[0], BIG_INTS[1]);
}

int32_t (*testcases[])() = {
    testcase1,
    testcase2,
    testcase3,
    testcase4,
    testcase5,
    testcase6,
    testcase7,
    testcase8,
    testcase9,
};
int32_t (*errorcases[])() = {
    testErrorSet,
    testErrorAdd,
    testErrorDel,
};

int32_t casePoints[] = {
    4,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
    2,
};
int32_t errorPoints[] = {
    1,
    1,
    1,
};

const int32_t cases = sizeof(testcases) / sizeof(testcases[0]);
const int32_t errors = sizeof(errorcases) / sizeof(errorcases[0]);

int main() {
    int32_t testType;
    scanf("%d", &testType);

    if (testType == TEST_TYPE_DEFAULT) {
        int32_t pass[cases];
        memset(pass, 0, sizeof(pass));
        int32_t points = 0;
        for (int32_t i = 0; i < cases; i++) {
            memset(&env, 0, sizeof(env));
            int r = sigsetjmp(env, 1);
            switch (r) {
                case 0: {
                    signal(SIGSEGV, recvSig);
                    printf("Testcase %d:\n", i + 1);
                    int32_t ret = testcases[i]();
                    if (ret == 0) {
                        points += casePoints[i];
                        pass[i] = 1;
                    }
                    break;
                }
                default:
                    fprintf(stderr, "Segmentation fault when running testcase %d\n", i + 1);
                    break;
            }
            printf("\n※※※※※※※※※※\n");
        }
        printf("Pass: ");
        for (int32_t i = 0; i < cases; i++) {
            if (pass[i]) {
                printf("%d ", i + 1);
            }
        }
        printf("\n");
        printf("Fail: ");
        for (int32_t i = 0; i < cases; i++) {
            if (!pass[i]) {
                printf("%d ", i + 1);
            }
        }
        printf("\n");
        printf("Points: %d\n", points);
    } else {
        int32_t deduce = 0;
        for (int32_t i = 0; i < errors; i++) {
            memset(&env, 0, sizeof(env));
            int r = sigsetjmp(env, 1);
            switch (r) {
                case 0: {
                    signal(SIGSEGV, recvSig);
                    printf("Errorcase %d:\n", i + 1);
                    int32_t ret = errorcases[i]();
                    if (ret != 0) {
                        deduce += errorPoints[i];
                    }
                    break;
                }
                default:
                    fprintf(stderr, "Segmentation fault when running errorcase %d\n", i + 1);
                    deduce += errorPoints[i];
                    break;
            }
        }
        printf("Deduce points: %d\n", deduce);
    }
    return 0;
}
