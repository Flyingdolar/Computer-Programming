#include "zip.h"

// ZIP Functions

bool isZIP(char *fName, FILE *fp) {
    uint32_t signature = 0;
    fread(&signature, sizeof(uint32_t), 1, fp);
    rewind(fp);
    if (strcmp(&fName[strlen(fName) - 3], "zip")) return false;
    if (signature == 0x04034b50) return true;
    return false;
}

char *getFileName(FILE **fp) {
    char *file_name = NULL;
    zipHead head;

    if (feof(*fp)) return NULL;
    fread(&head, sizeof(zipHead), 1, *fp);
    if (head.signature != 0x04034b50) return NULL;

    file_name = (char *)malloc(head.file_name_length + 1);
    fread(file_name, head.file_name_length, 1, *fp);
    file_name[head.file_name_length] = '\0';
    fseek(*fp, head.extra_field_length, SEEK_CUR);
    fseek(*fp, head.compressed_size, SEEK_CUR);

    PRINT_D("Name is %s\n", file_name);
    // printHead(&head);

    return file_name;
}

// Node Functions

pFile init_node(fileType type) {
    pFile ret = (pFile)malloc(sizeof(sFile));
    ret->name = NULL;
    ret->type = type;
    ret->len = 0;
    ret->Next = NULL;
    ret->Child = NULL;
    return ret;
}

void insert_node(pFile head, pFile node) {
    pFile p = head;
    while (p->Next != NULL) p = p->Next;
    p->Next = node;
    return;
}

pFile search_node(pFile head, char *name) {
    pFile p = head->Next;
    while (p != NULL) {
        if (!strcmp(p->name, name)) return p;
        p = p->Next;
    }
    return NULL;
}

void free_node(pFile *f) {
    if ((*f)->name) free((*f)->name);
    if ((*f)->Child) free_node(&((*f)->Child));
    if ((*f)->Next) free_node(&((*f)->Next));
    free(*f);
    *f = NULL;
    return;
}

// Sort Functions

void mgSort(pFile Arr, int32_t (*cond)(pFile, pFile)) {
    if (Arr->len <= 1) return;

    int32_t len = Arr->len;
    pFile arr_L = init_node(THEAD), arr_R = init_node(THEAD);
    pFile pArr = Arr, pL = arr_L, pR = arr_R;

    arr_L->len = len / 2, arr_R->len = len - arr_L->len;
    arr_L->Next = Arr->Next, arr_R->Next = Arr->Next;
    for (size_t idx = 0; idx < (size_t)arr_L->len; idx++)
        arr_R->Next = arr_R->Next->Next;

    mgSort(arr_L, cond);
    mgSort(arr_R, cond);
    pL = arr_L->Next, pR = arr_R->Next;

    while (pL != NULL || pR != NULL) {
        if (pL && pR) {
            if (cond(pL, pR)) {
                pArr->Next = pL;
                pArr = pArr->Next, pL = pL->Next;
            } else {
                pArr->Next = pR;
                pArr = pArr->Next, pR = pR->Next;
            }
        } else if (pL) {
            pArr->Next = pL;
            break;
        } else if (pR) {
            pArr->Next = pR;
            break;
        }
    }
    free_node(&arr_L), free_node(&arr_R);
    return;
}
