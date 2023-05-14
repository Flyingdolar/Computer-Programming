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

    // PRINT_D("[%s]\n", file_name);
    // printHead(&head);

    return file_name;
}

// Tree Functions

void setIndent(pFile root) {
    pFile ptr = root->Next;
    while (ptr != NULL) {
        strcpy(ptr->inStr, root->inStr);
        if (ptr->type == TDIR) {
            strcpy(ptr->Child->inStr, ptr->inStr);
            if (ptr->Next) strcat(ptr->Child->inStr, "|");
            if (ptr->Next == NULL) strcat(ptr->Child->inStr, " ");
            for (size_t idx = 0; idx < (strlen(ptr->name) + 7); idx++)
                strcat(ptr->Child->inStr, " ");
            setIndent(ptr->Child);
        }
        ptr = ptr->Next;
    }
    return;
}

pFile createTree(FILE *fp) {
    pFile root = init_node(THEAD);
    pFile ptr = root;
    char *fullPath = NULL, **path;
    int pathLen = 0;

    while ((fullPath = getFileName(&fp)) != NULL) {
        pFile node = init_node(TFILE);
        if (fullPath[strlen(fullPath) - 1] == '/') {
            node->type = TDIR;
            node->Child = init_node(THEAD);
            node->Child->Parent = node;
        }
        path = splitPath(fullPath, &pathLen);
        node->name = strdup(path[pathLen - 1]);
        for (int idx = 0; idx < pathLen - 1; idx++) {
            ptr = search_node(ptr, path[idx]);
            if (ptr == NULL) return NULL;
            ptr = ptr->Child;
        }
        insert_node(ptr, node), ptr->len++, ptr = root;
        freePath(path, pathLen);
    }
    setIndent(root);
    return root;
}

void sortTree(pFile root, int32_t (*cond)(pFile, pFile)) {
    mgSort(root, cond);
    while (root->Next != NULL) {
        root = root->Next;
        if (root->type != TDIR) break;
        sortTree(root->Child, cond);
    }
    return;
}

void printTree(pFile root) {
    pFile ptr = root->Next;
    while (ptr != NULL) {
        if (ptr->Prev != root) printf("%s", ptr->inStr);
        printf("+-- %s", ptr->name);
        if (ptr->type == TDIR) printf("/ --"), printTree(ptr->Child);
        if (ptr->type == TFILE) printf("\n");
        if (ptr->Next != NULL) printf("%s|\n", ptr->inStr);
        ptr = ptr->Next;
    }
    return;
}

// String Functions

char **splitPath(char *path, int *len) {
    char **ret = (char **)malloc(sizeof(char *));
    char str[1000], *ptr;

    *len = 0, strcpy(str, path);
    *ret = strdup(strtok(str, "/")), (*len)++;
    while ((ptr = strtok(NULL, "/")) != NULL) {
        ret = (char **)realloc(ret, sizeof(char *) * ((*len) + 1));
        ret[*len] = strdup(ptr), (*len)++;
    }
    free(path);
    return ret;
}

void freePath(char **path, int len) {
    if (path == NULL) return;
    for (int idx = 0; idx < len; idx++)
        if (path[idx] != NULL) free(path[idx]);
    free(path), path = NULL;
    return;
}

// Node Functions

pFile init_node(fileType type) {
    pFile ret = (pFile)malloc(sizeof(sFile));
    ret->name = NULL, ret->inStr[0] = '\0';
    ret->len = 0, ret->type = type;
    ret->Prev = NULL, ret->Next = NULL;
    ret->Parent = NULL, ret->Child = NULL;
    return ret;
}

void insert_node(pFile head, pFile node) {
    pFile p = head;
    while (p->Next != NULL) p = p->Next;
    p->Next = node, node->Prev = p;
    return;
}

pFile search_node(pFile head, char *name) {
    pFile p = head->Next;
    while (p != NULL) {
        if (strcmp(p->name, name) == 0) return p;
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

int32_t cmpType(pFile a, pFile b) {
    if (a->type == TDIR && b->type == TFILE) return 1;
    if (a->type == TFILE && b->type == TDIR) return 0;
    return 1;
}

int32_t cmpAsc(pFile a, pFile b) {
    if (a->type == TDIR && b->type == TFILE) return 1;
    if (a->type == TFILE && b->type == TDIR) return 0;
    if (strcmp(a->name, b->name) > 0) return 0;
    return 1;
}

int32_t cmpDsc(pFile a, pFile b) {
    if (a->type == TDIR && b->type == TFILE) return 1;
    if (a->type == TFILE && b->type == TDIR) return 0;
    if (strcmp(a->name, b->name) < 0) return 0;
    return 1;
}

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
