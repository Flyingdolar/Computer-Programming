#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 8192

typedef enum _position_ {
    ls = 0, st = 1, rs = 2, lw = 3, lf = 4, cf = 5,
    rf = 6, rw = 7, lam = 8, cam = 9, ram = 10, lm = 11,
    lcm = 12, cm = 13, rcm = 14, rm = 15, lwb = 16,
    ldm = 17, cdm = 18, rdm = 19, rwb = 20, lb = 21,
    lcb = 22, cb = 23, rcb = 24, rb = 25, gk = 26,
    err = -1
} position;

typedef struct _str_set_ *ptrset;
typedef struct _str_set_ {
    char *str;
    ptrset Next;
} strset;


typedef struct _player_ {
    int32_t id;
    char *name;
    char *club;
    position pos;
    int32_t ability[27];
    double value;
    double wage;
} player;

const char strPos[27][10] = {
    {"ls"}, {"st"}, {"rs"}, {"lw"}, {"lf"}, {"cf"},
    {"rf"}, {"rw"}, {"lam"}, {"cam"}, {"ram"}, {"lm"},
    {"lcm"}, {"cm"}, {"rcm"}, {"rm"}, {"lwb"},
    {"ldm"}, {"cdm"}, {"rdm"}, {"rwb"}, {"lb"},
    {"lcb"}, {"cb"}, {"rcb"}, {"rb"}, {"gk"}
};

position strtop(char *str);
char **strpart(char *str, char *del, size_t *num);
char **strppt(char *str, char *del, size_t *num);
int32_t toAby(char *strAby);
int isNA(char *playID, int32_t picked[11]);
int isOverBudget(char *playPrice, double budget);
int isPickBetter(char **pData, player Pick, position prop);
int isLowAbility(char *playAby, int32_t pickAby);
int isExpensive(char *playPrice, double pickPrice);
int isLatest(char *playID, int32_t pickID);
player pickPlayer(FILE *Index, int32_t NA[11], position prop, double price);

int ps_setFile(FILE **SetPos) {
    char fileName[BUFFER_SIZE];

    printf("\033[0;35mPlease enter the dataset: \033[0m");
    fgets(fileName, BUFFER_SIZE, stdin);
    fileName[strlen(fileName) - 1] = 0;
    if ((*SetPos = fopen(fileName, "r")) == NULL) return 1;
    return 0;
}

int ps_setPos(position Props[11]) {
    char playerInfo[BUFFER_SIZE], **playList;
    size_t playNum = 0;

    printf("\033[0;35mPlease enter the squad: \033[0m");
    fgets(playerInfo, BUFFER_SIZE, stdin);
    playList = strpart(playerInfo, " \n", &playNum);
    if (playNum != 11) return 13;

    for (size_t idx = 0; idx < 11; idx++){
        Props[idx] = strtop(playList[idx]);
        if (Props[idx] == err) return (idx + 1);
        if (idx < 10 && Props[idx] == gk)
            return 14;
    }
    if (Props[10] != gk) return 15;
    return 0;
}

int ps_setPrice(position Props[11], double Prices[11]) {
    fflush(stdin);
    printf("\033[0;35mBudget: \033[0m\n");
    for (size_t idx = 0; idx < 11; idx++){
        printf("    %s: ", strPos[Props[idx]]);
        scanf("%lf", &Prices[idx]);
    }
    return 0;
}

int ps_getMember(FILE *List, position Props[11], double Prices[11]) {
    player Pick[11];
    char Trash[BUFFER_SIZE];
    int32_t state = 1, chose[11] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};

    printf("\033[0;35mTeam: \033[0m\n");
    for (size_t idx = 0; idx < 11; idx++) {
        fgets(Trash, BUFFER_SIZE, List);
        Pick[idx] = pickPlayer(List, chose, Props[idx], Prices[idx]);
        if (Pick->id == -1) return (-1-idx);
        chose[idx] = Pick[idx].id;
        rewind(List);
    }

    for (size_t idx = 0; idx < 11; idx++) {
        if (Pick[idx].id == -1) return -1;
        printf("    %-*s: ", 3, strPos[Pick[idx].pos]);
        printf("%s/%s/", Pick[idx].name, Pick[idx].club);
        printf("%d/", Pick[idx].ability[Pick[idx].pos]);
        printf("%.0f/%.0f\n", Pick[idx].value, Pick[idx].wage);
    }
    return 0;
}

int main() {
    int32_t pid;
    FILE *FIFA20;
    position propList[11];
    double priceList[11];

    // Open File
    do {pid = ps_setFile(&FIFA20);
        if (pid == 1) printf("\033[1;33mWarning: \033[0;33mFailed to open the File, please try again.\n");
    } while (pid > 0);

    // Set Squad Players' Position
    if (pid < 0) return -1;
    do {pid = ps_setPos(propList);
        if (pid >= 1 && pid <= 11) printf("\033[1;33mWarning: \033[0;33mWrong position format at no.%d player, please try again.\n", pid);
        if (pid == 13) printf("\033[1;33mWarning: \033[0;33mSquad members is not \"11\", please try again.\n");
        if (pid == 14) printf("\033[1;33mWarning: \033[0;33mGoal Keeper is not at the last position.\n");
        if (pid == 15) printf("\033[1;33mWarning: \033[0;33mThe last member is not a goal keeper.\n");
    } while (pid > 0);

    // Set Squad Players' Price
    if (pid < 0) return -1;
    do {pid = ps_setPrice(propList, priceList);
        if (pid >= 1) printf("\033[1;33mWarning: \033[0;33mInvalid value on no.%d player's price, please try again.\n", pid);
    } while (pid > 0);

    // Find the Best Squad base on Budget
    if (pid < 0) return -1;
    do {pid = ps_getMember(FIFA20, propList, priceList);
        if (pid < 0) printf("\n\033[1;31mError: \033[0;31mNo Sufficient Money to Get player.\n");
    } while (pid > 0);
    return 0;
}

player pickPlayer(FILE *Index, int32_t NA[11], position prop, double price) {
    player Picked = {.pos = prop, .ability = 0, .id = -1, .value = price};
    char playerData[BUFFER_SIZE], **pData;
    size_t len;

    while (fgets(playerData, BUFFER_SIZE, Index)) {
        pData = strppt(playerData, ",", &len);
        if (isNA(pData[0], NA)) continue;
        if (isOverBudget(pData[7], price)) continue;
        if (isPickBetter(pData, Picked, prop)) continue;
        Picked.id = atoi(pData[0]);
        Picked.name = strdup(pData[2]);
        Picked.club = strdup(pData[14]);
        Picked.value = atof(pData[7]);
        Picked.wage = atof(pData[8]);
        for (size_t idx = 0; idx < 27; idx++)
            Picked.ability[idx] = toAby(pData[78 + idx]);
    }
    return Picked;
}

int isNA(char *playID, int32_t picked[11]) {
    if (playID == NULL) return 1;
    int32_t pID = atoi(playID);
    for (size_t idx = 0; idx < 11; idx++)
        if (pID == picked[idx])
            return 1;
    return 0;
}

int isOverBudget(char *playPrice, double budget) {
    if (playPrice == NULL) return 1;
    double price = atof(playPrice);
    if (price > budget) return 1;
    return 0;
}

int isPickBetter(char **pData, player Pick, position prop){
    int32_t pID = atoi(pData[0]);
    int32_t ability = toAby(pData[78 + prop]);
    double price = atof(pData[7]);

    if (ability < Pick.ability[prop]) return 1;
    if (ability > Pick.ability[prop]) return 0;
    if (price > Pick.value) return 1;
    if (price < Pick.value) return 0;
    if (pID > Pick.id) return 1;
    return 0;
};

int32_t toAby(char *strAby) {
    size_t num = 1;
    int32_t ability = 0;
    if (strAby == NULL) return ability;
    char **abList = strpart(strAby, "+", &num);
    for (size_t idx = 0; idx < num; idx++)
        ability += atoi(abList[idx]);
    return ability;
}

position strtop(char* str) {
    for (size_t idx = 0; idx < 27; idx++)
        if (strcmp(strPos[idx], str) == 0)
            return idx;
    return err;
}

char **strpart(char *str, char *del, size_t *num) {
    while (strchr(del, *str)) str++;
    char **List;
    size_t len = strlen(str);
    (*num) = 1;

    for (size_t idx = 0; idx < len; idx++)
        if (strchr(del, str[idx])) str[idx] = 0, (*num)++;
    List = (char **)malloc((*num) * sizeof(char *));

    (*num) = 1, List[0] = str;
    for (size_t idx = 1; idx < len; idx++)
        if (str[idx] != 0 && str[idx - 1] == 0)
            List[(*num)] = &str[idx], (*num)++;
    List = (char **)realloc(List, (*num) * sizeof(char *));
    return List;
}

char **strppt(char *str, char *del, size_t *num) {
    while (strchr(del, *str)) str++;
    char **List;
    size_t len = strlen(str), flag = 0;
    (*num) = 1;

    if (str == NULL) {
        List = (char **)malloc((*num) * sizeof(char *));
        *List = NULL;   return List;
    }
    for (size_t idx = 0; idx < len; idx++) {
        if (strchr("\"", str[idx])) { flag = (flag + 1) % 2; continue; }
        if (flag == 1) continue;
        if (strchr(del, str[idx])) str[idx] = 0, (*num)++;
    }
    List = (char **)malloc((*num) * sizeof(char *));

    (*num) = 1, List[0] = str;
    for (size_t idx = 1; idx < len; idx++)
        if (str[idx] == 0) {
            if (str[idx+1] != 0) List[(*num)] = &str[idx+1];
            else List[(*num)] = NULL;
            (*num)++;
        }
    return List;
}
