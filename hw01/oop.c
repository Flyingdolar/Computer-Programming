#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Potato;

enum MagicalEffect {
    EFF_Haste = 0,
    EFF_Slowness,
    EFF_Hunger,
    EFF_Saturation
};

static const char *MagicalEffectToString[] = {
    "Haste",
    "Slowness",
    "Hunger",
    "Saturation",
};

struct Potato {
    void (*print)(const struct Potato *);
    // Destruct potato and turn it into potato ball
    struct PotatoBall *(*fry)(struct Potato **);
    int32_t weight;
    const char *producer_name;
};

struct MagicalPotato {
    struct Potato base;
    enum MagicalEffect effect;
    int effect_level;
};

struct PotatoBall {
    void (*print)(const struct PotatoBall *);
    void (*dtor)(struct PotatoBall **);
    const char *producer_name;
    int32_t weight;
};

struct MagicalPotatoBall {
    struct PotatoBall base;
    enum MagicalEffect effect;
    int effect_level;
};

// Potato farmer or summoner

struct PotatoProducer {
    struct Potato *(*produce)(const struct PotatoProducer *);
    char name[32];
};

void print_potato(const struct Potato *this) {
    if (this == NULL) return;
    printf("\033[34m\033[4m\033[1mPotato:\033[0m\n");
    printf(" - Weight: %d g\n", this->weight);
    printf(" - Produced By: %s\n", this->producer_name);
    return;
};

void print_magical_potato(const struct Potato *this) {
    if (this == NULL) return;
    struct MagicalPotato *magicPtt = (struct MagicalPotato *)this;
    printf("\033[34m\033[4m\033[1mMagical Potato:\033[0m\n");
    printf(" - Weight: %d g\n", magicPtt->base.weight);
    printf(" - Produced By: %s\n", magicPtt->base.producer_name);
    printf(" - Magic Effect: %s\n", MagicalEffectToString[magicPtt->effect]);
    printf(" - Magic Level: %d\n", magicPtt->effect_level);
    return;
};

// No longer Used Function

void potato_dtor(struct Potato **obj) {
    printf("Warning: This function is no longer used, you should use \"fry\" instead to remove potatos.");
    if (!(*obj)) return;
    free(*obj);
    obj = NULL;
    return;
};

void magical_potato_dtor(struct Potato **obj) {
    printf("Warning: This function is no longer used, you should use \"fry\" instead to remove potatos.");
    if (!(*obj)) return;
    free(*obj);
    obj = NULL;
    return;
};

// Print Potato Balls

void print_potato_ball(const struct PotatoBall *this) {
    if (this == NULL) return;
    printf("\033[33m\033[4m\033[1mPotato Ball:\033[0m\n");
    printf(" - Weight: %d g\n", this->weight);
    printf(" - Produced By: %s\n", this->producer_name);
    return;
};

void print_magical_potato_ball(const struct PotatoBall *this) {
    if (this == NULL) return;
    struct MagicalPotatoBall *magicBall = (struct MagicalPotatoBall *)this;
    printf("\033[35m\033[4m\033[1mMagical Potato Ball:\033[0m\n");
    printf(" - Weight: %d g\n", magicBall->base.weight);
    printf(" - Produced By: %s\n", magicBall->base.producer_name);
    printf(" - Magic Effect: %s\n", MagicalEffectToString[magicBall->effect]);
    printf(" - Magic Level: %d\n", magicBall->effect_level);
    return;
};

// Delete Potato Balls

void potato_ball_dtor(struct PotatoBall **obj) {
    if (!(*obj)) return;
    free(*obj);
    obj = NULL;
    return;
};

void magical_potato_ball_dtor(struct PotatoBall **obj) {
    if (!(*obj)) return;
    free(*obj);
    obj = NULL;
    return;
};

// Fried Potatos into Potato Balls

struct PotatoBall *fry_potato_ball(struct Potato **input) {
    if (!(*input)) return NULL;
    struct PotatoBall *normalBall = (struct PotatoBall *)malloc(sizeof(struct PotatoBall));
    normalBall->producer_name = (*input)->producer_name;
    normalBall->weight = (*input)->weight;
    normalBall->print = print_potato_ball;
    normalBall->dtor = potato_ball_dtor;

    free(*input);
    *input = NULL;
    return normalBall;
};

struct PotatoBall *fry_magical_potato_ball(struct Potato **input) {
    if (!(*input)) return NULL;
    struct MagicalPotatoBall *magicBall = (struct MagicalPotatoBall *)malloc(sizeof(struct MagicalPotatoBall));
    struct MagicalPotato *mPotato = (struct MagicalPotato *)(*input);
    magicBall->effect = mPotato->effect;
    magicBall->effect_level = mPotato->effect_level;
    magicBall->base.producer_name = mPotato->base.producer_name;
    magicBall->base.weight = mPotato->base.weight;
    magicBall->base.print = print_magical_potato_ball;
    magicBall->base.dtor = magical_potato_ball_dtor;

    free(mPotato);
    mPotato = NULL;
    return (struct PotatoBall *)magicBall;
};

// Summon & Produce Potatoes

struct Potato *subarya_produce_potato(const struct PotatoProducer *this) {
    if (!this) return NULL;
    struct Potato *ptProduce = (struct Potato *)malloc(sizeof(struct Potato));
    ptProduce->producer_name = this->name;
    ptProduce->weight = 0;  // TODO: Should be Random
    ptProduce->fry = fry_potato_ball;
    ptProduce->print = print_potato;

    return ptProduce;
};

struct Potato *smallten_summon_potato(const struct PotatoProducer *this) {
    if (!this) return NULL;
    struct MagicalPotato *ptSummon = (struct MagicalPotato *)malloc(sizeof(struct MagicalPotato));
    ptSummon->effect = EFF_Haste;  // TODO: Should be Random
    ptSummon->effect_level = 2;    // TODO: Should be Random
    ptSummon->base.producer_name = this->name;
    ptSummon->base.weight = 0;  // TODO: Should be Random
    ptSummon->base.fry = fry_magical_potato_ball;
    ptSummon->base.print = print_magical_potato;

    return (struct Potato *)ptSummon;
};

// Initialize Character

void init_subarya(struct PotatoProducer *obj) {
    if (!obj) return;
    strcpy(obj->name, "subarya");
    obj->produce = subarya_produce_potato;
    return;
};

void init_smallten(struct PotatoProducer *obj) {
    if (!obj) return;
    strcpy(obj->name, "smallten");
    obj->produce = smallten_summon_potato;
    return;
};
