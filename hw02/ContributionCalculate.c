#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#define BUFFER_SIZE 8192

typedef enum _Month_ {
    Jan = 1, Feb = 2, Mar = 3, Apr = 4, May = 5, Jun = 6,
    Jul = 7, Aug = 8, Sep = 9, Oct = 10, Nov = 11, Dec = 12,
    err = -1, empty = 0
} Month;

typedef struct _author_ *pAuth;
typedef struct _author_ {
    int32_t fileNum;
    int32_t insert;
    int32_t delete;
} sAuth;

typedef struct _msg_ *pMsg;
typedef struct _msg_ {
    char *id;
    char *author;
    char *email;
    int32_t time[6];
    int32_t fileNum;
    int32_t insert;
    int32_t delete;
    pMsg Next;
} sMsg;

const char str_M[12][10] = {
    {"Jan"},{"Feb"},{"Mar"},{"Apr"},{"May"},{"Jun"},
    {"Jul"},{"Aug"},{"Sep"},{"Oct"},{"Nov"},{"Dec"}
};

int toNum(char letter);
Month toMonth(char *strMonth);
void read_title(char *msg, pMsg *Node);
void read_author(char *msg, pMsg *Node);
void read_date(char *msg, pMsg *Node);
void read_change(char *msg, pMsg *Node);
void writeCommit(pMsg data, const char *val, int mode);

pMsg parseMsg(char *fileName){
    FILE *Gitlog = fopen(fileName, "r");
    pMsg List = (pMsg)malloc(sizeof(sMsg)), Mtr = List;
    char rowMsg[BUFFER_SIZE];

    if (Gitlog == NULL) return NULL;
    while (fgets(rowMsg, BUFFER_SIZE, Gitlog)) {
        if (rowMsg[0] == 'c') read_title(rowMsg, &Mtr);
        if (rowMsg[0] == 'A') read_author(rowMsg, &Mtr);
        if (rowMsg[0] == 'D') read_date(rowMsg, &Mtr);
        if (rowMsg[0] == ' ' && rowMsg[1] != 0) {
            if (toNum(rowMsg[1]) >= 0) read_change(rowMsg, &Mtr);
        }
    }

    fclose(Gitlog);
    return List;
}

void searchCommitInformationByHashVal(const char *hashVal) {
    pMsg Data = parseMsg("frontend.txt"), Target = NULL;
    size_t hashLen = strlen(hashVal);

    for (size_t idx = 0; Data != NULL; Data = Data->Next){
        if (idx == 0) continue;
        if (strncmp(hashVal, Data->id, hashLen) == 0) {
            if (Target != NULL) {
                writeCommit(NULL, hashVal, -1);
                return;
            }
            Target = Data;
        }
    }
    if (Target != NULL) {
        writeCommit(Target, hashVal, 1);
        return;
    }
    writeCommit(NULL, hashVal, 0);
    return;
}

void searchMonthlyContribution(const char *monthAbbrev);

int toNum(char letter) {
    int32_t num = letter - '0';
    if (num < 0 || num > 9) num = -1;
    return num;
}

Month toMonth(char *strMonth) {
    for (size_t idx = 0; idx < 12; idx++){
        if (strncmp(strMonth, str_M[idx], 3) == 0)
            return (idx + 1);
    }
    return err;
}

void writeCommit(pMsg data, const char *val, int mode) {
    FILE *Output;
    Output = fopen("contribution.txt", "w");
    if (Output == NULL) return;

    fprintf(Output, "\n(%s)", val);

    if (mode <= 0) {
        if (mode == 0) fprintf(Output, " Not found.\n");
        if (mode == -1) fprintf(Output, " More than two search results.\n");
        return;
    }
    fprintf(Output, "\n");
    fprintf(Output, "- %s\n", data->author);
    fprintf(Output, "    - %s\n", data->email);
    fprintf(Output, "    - %d/%02d/%02d/%02d:%02d:%02d",
        data->time[0], data->time[1], data->time[2], data->time[3], data->time[4], data->time[5]);
    fprintf(Output, "    - %d file changed\n", data->fileNum);
    fprintf(Output, "    - %d insertions\n", data->insert);
    fprintf(Output, "    - %d deletions\n", data->delete);

    fclose(Output);
}

void read_title(char *msg, pMsg *Node) {
    pMsg Commit = (pMsg)malloc(sizeof(sMsg));

    if ((*Node) == NULL) {free(Commit); return;}
    Commit->id = strdup(&msg[7]);
    Commit->fileNum = 0, Commit->insert = 0, Commit->delete = 0;
    Commit->Next = NULL;

    (*Node)->Next = Commit;
    (*Node) = (*Node)->Next;
    return;
}

void read_author(char *msg, pMsg *Node) {
    size_t len = strlen(msg);
    char *name = &msg[8], *mail;

    if ((*Node) == NULL) return;
    for (size_t idx = 0; idx < len; idx++){
        if (msg[idx] == '<') {
            msg[idx] = 0, msg[idx-1] = 0;
            mail = &msg[idx+1];
        }
        if (msg[idx] == '>') msg[idx] = 0;
    }
    (*Node)->author = strdup(name);
    (*Node)->email = strdup(mail);
    return;
}

void read_date(char *msg, pMsg *Node) {
    char *time = strtok(msg, " :");
    time = strtok(NULL, " :");

    if ((*Node) == NULL) return;
    for (size_t idx = 0; idx < 6; idx++){
        time = strtok(NULL, " :");
        if (idx == 0) (*Node)->time[1] = toMonth(time);
        if (idx == 5) (*Node)->time[0] = atoi(time);
        if (idx > 0 && idx < 5) (*Node)->time[idx+1] = atoi(time);
    }
    return;
}

void read_change(char *msg, pMsg *Node) {
    size_t len = strlen(msg);
    char *numStr;

    if ((*Node) == NULL) return;
    for (size_t idx = 0; idx < len; idx++){
        if (strncmp(&msg[idx], "file", 4) == 0)
            msg[idx-1] = 0, (*Node)->fileNum = atoi(msg);
        if (strncmp(&msg[idx], "ins", 3) == 0) {
            msg[idx-1] = 0, numStr = &msg[idx-2];
            while (toNum(*numStr) >= 0) numStr--;
            (*Node)->insert = atoi(numStr + 1);
        }
        if (strncmp(&msg[idx], "del", 3) == 0) {
            msg[idx-1] = 0, numStr = &msg[idx-2];
            while (toNum(*numStr) >= 0) numStr--;
            (*Node)->delete = atoi(numStr + 1);
        }
    }
    return;
}
