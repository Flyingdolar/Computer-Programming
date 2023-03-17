#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _object_ sObject;
typedef sObject* pObject;
typedef struct _content_ sContent;
typedef sContent* pContent;

typedef enum {
    object = 0,
    string = 1,
    boolean = 2,
    integer = 3,
    fraction = 4,
} jType;

struct _object_ {
    char* name;
    pContent Child;
    pObject Next;
};

struct _content_ {
    jType type;
    char* data;
    pObject ObList;
    pContent Next;
};

pObject createObj();
pContent createContent();

void JSON_INIT();
pObject JSON_READ();
int32_t JSON_GET();
int32_t JSON_CLEAN();

int main() {
    char strJson[8193], strSelect[3];
    char searchKey[8193], *getData;
    int8_t userChoice = 1;

    // JSON_INIT();

    printf("Please enter the JSON string :\n");
    fgets(strJson, sizeof(strJson) - 1, stdin);
    strJson[strlen(strJson)] = '\0';
    // if (JSON_READ(strJson) == -1) {
    //     printf("Failed to load Json data\n");
    //     return -1;
    // };

    while (userChoice) {
        printf("Choice ( 0 : EXIT / 1 : GET ) : ");
        fgets(strSelect, 1, stdin);
        userChoice = atoi(strSelect);

        if (userChoice == 1) {
            printf("Enter  KEY  :  ");
            fgets(searchKey, sizeof(searchKey) - 1, stdin);
            searchKey[strlen(searchKey)] = '\0';
            // if (JSON_GET(searchKey) == -1) {
            //     printf("Failed to get data\n");
            //     return -1;
            // };
        };
    };
    // JSON_CLEAN();
    if (userChoice != 0) return -1;
    return 0;
};
