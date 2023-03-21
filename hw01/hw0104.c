#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    json_obj = 0,
    json_str = 1,
    json_bol = 2,
    json_int = 3,
    json_frc = 4,
} json_type;

typedef enum {
    NEXT = 1,    // , exepected
    ASSIGN = 2,  // : expected
    VALUE = 3,   // 1234567890TFtf expected
    STRING = 4,  // " exepected
    OBJECT = 5,  // { expected
    ARRAY = 6,   // [ expected
} str_cmd;

typedef struct _obj_* pObj;
typedef struct _key_* pKey;
typedef struct _obj_ {
    char* name;
    bool isArray;
    pKey Data;
    pObj Next;
} sObj;

// Represent the Data Value of the Object
typedef struct _key_ {
    json_type type;
    char* str;
    char* num;
    char* frac;
    bool is;
    pObj Obj;
    pKey Next;
} sKey;

pObj newObj();
pKey newKey();
bool isNextCmd(char** ppStr, str_cmd cmd);
char* parseWord(char** ppStr);
void parseNum(char** ppStr, pKey nodeKey);
pObj parseObj(char** ppStr);
void parseArray(char** ppStr, pObj object);
void printValue(const pKey value, bool isArray);
void printObj(const pObj object);

pObj newObj() {
    pObj new = (pObj)malloc(sizeof(sObj));
    new->isArray = false;
    new->Next = NULL;
    new->Data = NULL;
    return new;
}

pKey newKey() {
    pKey new = (pKey)malloc(sizeof(sKey));
    new->Next = NULL;
    new->Obj = NULL;
    new->is = false;
    return new;
}

// Search the Command : [ { " ,
bool isNextCmd(char** ppStr, str_cmd cmd) {
    char* cmdList = "\"[]{},:-1234567890TFtf";
    *ppStr += strcspn(*ppStr, cmdList) + 1;
    switch (cmd) {
        case ASSIGN:
            if (*(*ppStr - 1) == ':') return true;
            return false;
        case NEXT:
            if (*(*ppStr - 1) == ',') return true;
            return false;
        case STRING:
            if (*(*ppStr - 1) == '\"') return true;
            return false;
        case OBJECT:
            if (*(*ppStr - 1) == '{') return true;
            return false;
        case ARRAY:
            if (*(*ppStr - 1) == '[') return true;
            return false;
        case VALUE:
            if (strchr(cmdList, *(*ppStr - 1))) return true;
            return false;
    }
    return false;
}

// If Read " , End with "
char* parseWord(char** ppStr) {
    size_t len = strcspn(*ppStr, "\"");
    char* pick = (char*)malloc((len + 1) * sizeof(char));
    strncpy(pick, *ppStr, len);
    *ppStr += len + 1;
    return pick;
}

// If Read -0123456789.TFtf
void parseNum(char** ppStr, pKey nodeKey) {
    if (!(strncmp(*ppStr, "True", 4) && strncmp(*ppStr, "true", 4))) {
        nodeKey->type = json_bol;
        nodeKey->is = true;
        *ppStr += 4;
    } else if (!(strncmp(*ppStr, "False", 5) && strncmp(*ppStr, "false", 5))) {
        nodeKey->type = json_bol;
        *ppStr += 5;
    } else {
        nodeKey->type = json_int;
        // if (**ppStr == '-') nodeKey->neg = true;
        // if (nodeKey->neg) (*ppStr)++;
        size_t len = strspn(*ppStr, "-1234567890");
        nodeKey->num = strndup(*ppStr, len);
        (*ppStr) += len;
        if (**ppStr == '.') {
            nodeKey->type = json_frc;
            (*ppStr)++;

            len = strspn(*ppStr, "1234567890");
            nodeKey->frac = strndup(*ppStr, len);
            (*ppStr) += len;
        }
    }
    return;
}

// If Read { , End with }
pObj parseObj(char** ppStr) {
    str_cmd cmd;
    pObj object = newObj(), pNode = object;

    while (**ppStr != 0) {
        if (!(isNextCmd(ppStr, STRING))) printf("myEr: \" expected.");
        pNode->name = parseWord(ppStr);
        if (!(isNextCmd(ppStr, ASSIGN))) printf("myEr: : expected.");
        pNode->Data = newKey();
        if (!(isNextCmd(ppStr, VALUE))) printf("myEr: Value invalid.");
        if (*(*ppStr - 1) == '[') {
            pNode->isArray = true;
            parseArray(ppStr, pNode);
        } else if (*(*ppStr - 1) == '{') {
            pNode->Data->type = json_obj;
            pNode->Data->Obj = parseObj(ppStr);
        } else if (*(*ppStr - 1) == '\"') {
            pNode->Data->type = json_str;
            pNode->Data->str = parseWord(ppStr);
        } else {  // Deal with character: TFtf1234567890
            (*ppStr)--;
            parseNum(ppStr, pNode->Data);
        }
        if (!(isNextCmd(ppStr, NEXT))) return object;
        pObj next = newObj();
        pNode->Next = next;
        pNode = pNode->Next;
    }
    printf("myEr: } expected.");
    return NULL;
}

// If Read [ , End with ]
void parseArray(char** ppStr, pObj object) {
    pKey pNode = newKey();
    object->Data = pNode;
    while (**ppStr != 0) {
        if (!(isNextCmd(ppStr, VALUE))) printf("myEr: Value invalid.");
        if (*(*ppStr - 1) == '{') {
            pNode->type = json_obj;
            pNode->Obj = parseObj(ppStr);
        } else if (*(*ppStr - 1) == '\"') {
            pNode->type = json_str;
            pNode->str = parseWord(ppStr);
        } else {  // Deal with character: TFtf1234567890
            (*ppStr)--;
            parseNum(ppStr, pNode);
        }
        if (!(isNextCmd(ppStr, NEXT))) return;
        pKey next = newKey();
        pNode->Next = next;
        pNode = pNode->Next;
    }
}

void printValue(const pKey value, bool isArray) {
    pKey pNode = (pKey)value;
    if (isArray) printf("[ ");
    do {
        switch (pNode->type) {
            case json_str:
                printf("\"%s\"", pNode->str);
                break;
            case json_bol:
                if (pNode->is) printf("true");
                if (!(pNode->is)) printf("false");
                break;
            case json_int:
                printf("%s", pNode->num);
                break;
            case json_frc:
                printf("%s.%s", pNode->num, pNode->frac);
                break;
            case json_obj:
                printObj(pNode->Obj);
                break;
        }
        if (isArray && pNode->Next) printf(", ");
        pNode = pNode->Next;
    } while (isArray && pNode != NULL);
    if (isArray) printf(" ]");
    return;
}

void printObj(const pObj object) {
    pObj pNode = (pObj)object;
    printf("{ ");
    while (pNode != NULL) {
        printf("\"%s\" : ", pNode->name);
        printValue(pNode->Data, pNode->isArray);
        if (pNode->Next) printf(", ");
        pNode = pNode->Next;
    }
    printf(" }");
    return;
}

pKey searchKey(const pObj head, const char* keyStr) {
    pObj pNode = ((pObj)head)->Data->Obj;
    pKey pValue;
    char *pStr = strdup(keyStr), *idxStr;
    int32_t arrIndex = -1;
    pStr = strtok(pStr, ".");
    while (pStr) {
        if ((idxStr = strchr(pStr, '[')) != NULL) {
            arrIndex = 0, idxStr++;
            *(strchr(pStr, ']')) = 0;
            *(strchr(pStr, '[')) = 0;
            while (*idxStr) {
                arrIndex += arrIndex * 10 + ((*idxStr) - '0');
                idxStr++;
            }
        };
        while (pNode) {
            if (!(strcmp(pNode->name, pStr))) break;
            pNode = pNode->Next;
        };
        if (!pNode) return NULL;
        pValue = pNode->Data;
        if (arrIndex >= 0)
            while (arrIndex--) pValue = pValue->Next;
        pStr = strtok(NULL, ".");
        if (pStr) pNode = pValue->Obj;
    };
    return pValue;
}

void freeNode(pObj headNode) {
    pObj hPtr = headNode, nPtr;
    while (hPtr != NULL) {
        nPtr = hPtr->Next;
        if (hPtr->Data->type == json_obj)
            freeNode(hPtr->Data->Obj);
        free(hPtr->Data);
        free(hPtr);
        hPtr = nPtr;
    }
    return;
}

int main() {
    pObj Json = newObj();
    pKey vNode;
    int32_t len;
    bool isArray = false;
    char inStr[8194], keyStr[8194], choice[10];
    char *pStr = inStr, *kStr;

    Json->name = "Head";
    printf("Please enter the JSON string:\n");
    fgets(inStr, 8193, stdin);
    if (inStr[strlen(inStr) - 1] == '\n') inStr[strlen(inStr) - 1] = 0;

    pStr += (strcspn(pStr, "[{") + 1);
    if (*(pStr - 1) == '[') {
        printf("Here\n");
        Json->isArray = true;
        parseArray(&pStr, Json);
    }
    if (*(pStr - 1) == '{') {
        pKey node = newKey();
        node->type = json_obj;
        Json->Data = node;
        Json->Data->Obj = parseObj(&pStr);
    }

    do {
        printf("Choice (0:Exit ,1:Get) : ");
        fgets(choice, 9, stdin);
        if (choice[0] == '1') {
            printf("Key : ");
            fgets(keyStr, 8193, stdin);
            kStr = keyStr;
            if (kStr[strlen(kStr) - 1] == '\n') kStr[strlen(kStr) - 1] = 0;
            vNode = searchKey(Json, kStr);
            isArray = (keyStr[strlen(keyStr) - 1] == ']') ? false : true;
            if (vNode) {
                if (!(vNode->Next)) isArray = false;
                printValue(vNode, isArray);
            }
            if (!vNode) printf("(Not Found)");
            printf("\n");
        }
    } while (choice[0] == '1');

    freeNode(Json);
    return 0;
}
