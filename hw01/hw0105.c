#include <setjmp.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "oop.h"

sigjmp_buf env;

void segfault_handler(int sig) { siglongjmp(env, 1); }

int main() {
    signal(SIGSEGV, segfault_handler);

    int32_t sunny_cases = 0;

    // A1
    int jmp_ret = sigsetjmp(env, 1);
    if (!jmp_ret) {
        init_smallten(NULL);
        init_subarya(NULL);
        sunny_cases += 1;
    } else {
        printf("A1 ");
        fprintf(stderr, "[SIGSEGV]init smallten or subarya no null check.\n");
    }

    // A2
    jmp_ret = sigsetjmp(env, 1);
    if (!jmp_ret) {
        print_magical_potato(NULL);
        print_potato(NULL);
        sunny_cases += 1;
    } else {
        printf("A2 ");
        fprintf(stderr, "[SIGSEGV]print (magical) potato no null check.\n");
    }

    // A3
    jmp_ret = sigsetjmp(env, 1);
    if (!jmp_ret) {
        print_magical_potato_ball(NULL);
        print_potato_ball(NULL);
        sunny_cases += 1;
    } else {
        printf("A3 ");
        fprintf(stderr,
                "[SIGSEGV]print (magical) potato ball no null check.\n");
    }

    // A4
    jmp_ret = sigsetjmp(env, 1);
    if (!jmp_ret) {
        fry_magical_potato_ball(NULL);
        fry_potato_ball(NULL);
        sunny_cases += 1;
    } else {
        printf("A4 ");
        fprintf(stderr, "[SIGSEGV]fry (magical) potato ball no null check.\n");
    }

    // A5
    jmp_ret = sigsetjmp(env, 1);
    if (!jmp_ret) {
        smallten_summon_potato(NULL);
        subarya_produce_potato(NULL);
        sunny_cases += 1;
    } else {
        printf("A5 ");
        fprintf(stderr, "[SIGSEGV]produce potato no null check.\n");
    }

    // A6
    jmp_ret = sigsetjmp(env, 1);
    if (!jmp_ret) {
        potato_ball_dtor(NULL);
        magical_potato_ball_dtor(NULL);
        sunny_cases += 1;
    } else {
        printf("A6 ");
        fprintf(stderr,
                "[SIGSEGV]destruct (magical) potato ball no null check.\n");
    }

    // B*
    int32_t loss_score = 0;
    struct PotatoProducer smallten, subarya;
    jmp_ret = sigsetjmp(env, 1);
    if (!jmp_ret) {
        init_smallten(&smallten);
        init_subarya(&subarya);
    } else {
        printf("B* ");
        fprintf(stderr, "[SIGSEGV]Fail to init subarya or smallten.\n");
        loss_score += 1;
        goto FAIL;
    }

    // B1
    jmp_ret = sigsetjmp(env, 1);
    struct Potato *p;
    struct PotatoBall *pb;
    if (!jmp_ret) {
        p = smallten.produce(&smallten);
        p->print(p);
        jmp_ret = sigsetjmp(env, 1);
        if (!jmp_ret) {
            pb = p->fry(&p);  // p pointer set to NULL
            if (p != NULL) {
                printf("B1.1 ");
                fprintf(stderr,
                        "(Smallten) Potato frying didn't destroy potato.\n");
                loss_score += 1;  // B1.1
            }
            pb->print(pb);
            pb->dtor(&pb);  // pb pointer set to NULL
            if (pb != NULL) {
                printf("B1.2 ");
                fprintf(stderr, "(Smallten) Potato ball destroying failed.\n");
                loss_score += 1;  // B1.2
            }
        } else {
            printf("B1.3 ");
            fprintf(stderr, "[SIGSEGV](Smallten) Fail to fried potato ball.\n");
            loss_score += 1;  // B1.3
        }
    } else {
        printf("B1.4 ");
        fprintf(stderr, "[SIGSEGV]Smallten fail to summon potato.\n");
        loss_score += 1;  // B1.4
    }
    p = NULL;
    pb = NULL;

    // B2
    jmp_ret = sigsetjmp(env, 1);
    if (!jmp_ret) {
        p = subarya.produce(&subarya);
        p->print(p);
        jmp_ret = sigsetjmp(env, 1);
        if (!jmp_ret) {
            pb = p->fry(&p);  // p pointer set to NULL
            if (p != NULL) {
                printf("B2.1 ");
                fprintf(stderr,
                        "(Subarya) Potato frying didn't destroy potato.\n");
                loss_score += 1;
            }
            pb->print(pb);
            pb->dtor(&pb);  // pb pointer set to NULL
            if (pb != NULL) {
                printf("B2.2 ");
                fprintf(stderr, "(Subarya) Potato ball destroying failed.\n");
                loss_score += 1;
            }
        } else {
            printf("B2.3 ");
            fprintf(stderr, "[SIGSEGV](Smallten) Fail to fried potato ball.\n");
            loss_score += 1;
        }
    } else {
        printf("B2.4 ");
        fprintf(stderr, "[SIGSEGV]Subarya fail to farm potato.\n");
        loss_score += 1;
    }
    p = NULL;
    pb = NULL;

    // B3
    jmp_ret = sigsetjmp(env, 1);
    if (!jmp_ret) {
        strncpy(smallten.name, "Cutest Smallten", strlen("Cutest Smallten"));
        p = smallten.produce(&smallten);
        p->print(p);
        jmp_ret = sigsetjmp(env, 1);
        if (!jmp_ret) {
            pb = p->fry(&p);  // p pointer set to NULL
            if (p != NULL) {
                printf("B3.1 ");
                fprintf(
                    stderr,
                    "(Cutest Smallten) Potato frying didn't destroy potato.\n");
                loss_score += 1;
            }
            pb->print(pb);
            pb->dtor(&pb);  // pb pointer set to NULL
            if (pb != NULL) {
                printf("B3.2 ");
                fprintf(stderr,
                        "(Cutest Smallten) Potato ball destroying failed.\n");
                loss_score += 1;
            }
        } else {
            printf("B3.3 ");
            fprintf(stderr,
                    "[SIGSEGV](Cutest Smallten) Fail to fried potato ball.\n");
            loss_score += 1;
        }
    } else {
        printf("B3.4 ");
        fprintf(stderr, "[SIGSEGV]Cutest Smallten fail to summon potato.\n");
        loss_score += 1;
    }

    // B4
    jmp_ret = sigsetjmp(env, 1);
    if (!jmp_ret) {
        strncpy(subarya.name, "Subarray", strlen("Subarray"));
        p = subarya.produce(&subarya);
        p->print(p);
        jmp_ret = sigsetjmp(env, 1);
        if (!jmp_ret) {
            pb = p->fry(&p);  // p pointer set to NULL
            if (p != NULL) {
                printf("B4.1 ");
                fprintf(stderr,
                        "(Subarray) Potato frying didn't destroy potato.\n");
                loss_score += 1;
            }
            pb->print(pb);
            pb->dtor(&pb);  // pb pointer set to NULL
            if (pb != NULL) {
                printf("B4.2 ");
                fprintf(stderr, "(Subarray) Potato ball destroying failed.\n");
                loss_score += 1;
            }
        } else {
            printf("B4.3 ");
            fprintf(stderr, "[SIGSEGV](Subarray) Fail to fried potato ball.\n");
            loss_score += 1;
        }
    } else {
        printf("B4.4 ");
        fprintf(stderr,
                "[SIGSEGV]Subarray fail to array subscript a potato.\n");
        loss_score += 1;
    }

    printf("Loss Score: %d.\nSunny cases: %d.\nTotal: %d\n", loss_score,
           sunny_cases * 2, (8 - loss_score) + sunny_cases * 2);
    return 0;
FAIL:
    printf("Loss Score: %d.\nSunny cases: %d.\nTotal: %d\n", loss_score,
           sunny_cases * 2, (8 - loss_score) + sunny_cases * 2);
    return 1;
}
