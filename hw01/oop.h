#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

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

void print_potato(const struct Potato *this);
void print_magical_potato(const struct Potato *this);
void potato_dtor(struct Potato **obj);
void magical_potato_dtor(struct Potato **obj);

void print_potato_ball(const struct PotatoBall *this);
void print_magical_potato_ball(const struct PotatoBall *this);
void potato_ball_dtor(struct PotatoBall **obj);
void magical_potato_ball_dtor(struct PotatoBall **obj);

struct PotatoBall *fry_potato_ball(struct Potato **input);
struct PotatoBall *fry_magical_potato_ball(struct Potato **input);

struct Potato *smallten_summon_potato(const struct PotatoProducer *this);
struct Potato *subarya_produce_potato(const struct PotatoProducer *this);

void init_smallten(struct PotatoProducer *obj);
void init_subarya(struct PotatoProducer *obj);
