#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <unistd.h>

// enum Type in Directory Files
// DT_UNKNOWN = 0, DT_FIFO = 1, DT_CHR = 2, DT_DIR = 4, DT_BLK = 6
// DT_REG = 8, DT_LNK = 10, DT_SOCK = 12, DT_WHT = 14

typedef struct dirent DIR_prop;
typedef struct stat FILE_stat;
typedef struct winsize size_w;
typedef struct _sdoc_ *pdoc;
typedef struct _sdoc_ {
    char *name;
    char *path;
    int32_t type;
    bool isLink;
    pdoc Next;
} sdoc;

typedef struct _sQNode_ *pQNode;
typedef struct _sQNode_ {
    char *path;
    pQNode Next;
} sQNode;

typedef struct _Queue_ {
    pQNode Front;
    pQNode Rear;
} Queue;

// Comparison
bool comp_alpha(sdoc fileA, sdoc fileB);
// Queue
void enqueue(Queue *Qstack, pQNode node);
pQNode dequeue(Queue *Qstack);
// Paths History
void rec_history(char *path);
int32_t check_history(char *path);
// Directory Functions
pdoc newDoc(char *file_name, int32_t file_type);
pdoc read_dir(char *path, size_t *len);
void sort_dir(pdoc *List, bool (*comp)(sdoc, sdoc));
void print_dir(pdoc Dir_File, size_w wSize);

Queue FQue;
pQNode History;

void ListAllFile(char *path, size_w size) {
    size_t fileLen;
    pdoc fileList = read_dir(path, &fileLen), fileHead = fileList;
    pQNode nextFile;

    sort_dir(&fileList->Next, comp_alpha);
    while ((fileList = fileList->Next) != NULL) {
        if (fileList->name[0] == '.') continue;
        if (fileList->type == DT_DIR && check_history(fileList->path) < 0) {
            pQNode Folder = (pQNode)malloc(sizeof(sQNode));
            Folder->path = strdup(fileList->path);
            rec_history(Folder->path);
            enqueue(&FQue, Folder);
        }
    }

    fileList = fileHead;
    print_dir(fileList, size);
    if ((nextFile = dequeue(&FQue)) != NULL) {
        printf("\n");
        ListAllFile(nextFile->path, size);
    }
    return;
}

int main(int argc, char **argv) {
    char *path = ".";
    size_t len = 0;
    size_w size;

    if (argc > 1) path = argv[1];
    if (!isatty(STDOUT_FILENO)) return 1;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);

    rec_history(path);
    ListAllFile(path, size);
    return 0;
}

pdoc newDoc(char *file_name, int32_t file_type) {
    pdoc node = (pdoc)malloc(sizeof(sdoc));
    node->name = strdup(file_name), node->type = file_type;
    node->isLink = false, node->Next = NULL, node->path = NULL;
    return node;
}

bool comp_alpha(sdoc fileA, sdoc fileB) {
    if (strcmp(fileA.name, fileB.name) < 0) return true;
    return false;
}

pdoc read_dir(char *path, size_t *dirLen) {
    DIR *Dir_file;
    DIR_prop *Entry;
    FILE_stat state;
    pdoc List, Node;
    size_t pathLen;

    if ((Dir_file = opendir(path)) == NULL) return NULL;
    List = newDoc(".", DT_DIR), Node = List;
    List->path = strdup(path), (*dirLen) = 1;

    while ((Entry = readdir(Dir_file)) != NULL) {
        if (Entry->d_name[0] == '.') continue;
        Node->Next = newDoc(Entry->d_name, Entry->d_type);
        Node = Node->Next, (*dirLen)++;

        pathLen = strlen(path) + strlen(Node->name) + 2;
        Node->path = (char *)malloc(pathLen * sizeof(char));
        snprintf(Node->path, pathLen, "%s/%s", path, Node->name);

        if (Node->type == DT_LNK) {
            if (stat(Node->path, &state) < 0) break;
            Node->isLink = true;
            Node->type = DT_UNKNOWN;
            if (S_ISREG(state.st_mode)) Node->type = DT_REG;
            if (S_ISDIR(state.st_mode)) Node->type = DT_DIR;
            if (S_ISCHR(state.st_mode)) Node->type = DT_CHR;
            if (S_ISBLK(state.st_mode)) Node->type = DT_BLK;
            if (S_ISFIFO(state.st_mode)) Node->type = DT_FIFO;
            if (S_ISLNK(state.st_mode)) Node->type = DT_LNK;
            if (S_ISSOCK(state.st_mode)) Node->type = DT_SOCK;
        }
    }
    return List;
}

void sort_dir(pdoc *List, bool (*comp)(sdoc, sdoc)) {
    pdoc head = *List, mid;
    pdoc curr = (pdoc)malloc(sizeof(sdoc));
    pdoc slow = head, fast = head->Next;

    if (!head || !head->Next) return;
    while (fast != NULL) {
        fast = fast->Next;
        if (fast != NULL) slow = slow->Next, fast = fast->Next;
    }
    mid = slow->Next, slow->Next = NULL;
    sort_dir(&head, comp), sort_dir(&mid, comp);
    *List = curr;
    while (head && mid) {
        if (comp(*head, *mid)) {
            curr->Next = head;
            curr = curr->Next, head = head->Next;
        } else {
            curr->Next = mid;
            curr = curr->Next, mid = mid->Next;
        }
    }
    if (head) curr->Next = head;
    if (mid) curr->Next = mid;
    curr = *List, *List = (*List)->Next, free(curr);
    return;
}

void print_dir(pdoc Dir_File, size_w wSize) {
    pdoc file = Dir_File;
    int32_t fileLen, fileNum = 0;
    int32_t maxRow, maxCol, maxLen = 0, count = 0;

    while ((file = file->Next) != NULL) {
        if (file->name[0] == '.') continue;
        fileLen = strlen(file->name), fileNum++;
        if (fileLen > maxLen) maxLen = fileLen;
    }
    maxCol = wSize.ws_col / (maxLen + 3);
    maxRow = (fileNum / maxCol) + 1;
    if ((fileNum / maxRow) < maxCol) maxCol = fileNum / maxRow + 1;

    file = Dir_File;
    printf("\033[1;31m%s\033[0m:\n", file->path);
    while ((file = file->Next) != NULL) {
        if (file->name[0] == '.') continue;
        if (file->type == DT_DIR)
            printf("\033[1;34m%-*s\033[0m", maxLen + 2, file->name);
        else
            printf("\033[32m%-*s\033[0m", maxLen + 2, file->name);
        if ((++count) % maxCol == 0 || count == fileNum) printf("\n");
    }
    return;
}

void enqueue(Queue *Qstack, pQNode node) {
    node->Next = NULL;
    if (Qstack->Rear == NULL) {
        Qstack->Rear = node, Qstack->Front = node;
        return;
    }
    Qstack->Rear->Next = node, Qstack->Rear = node;
    return;
}

pQNode dequeue(Queue *Qstack) {
    pQNode detNode = Qstack->Front;

    if (Qstack->Front == NULL) return NULL;
    Qstack->Front = Qstack->Front->Next;
    return detNode;
}

void rec_history(char *path) {
    pQNode hisPtr, hisNode = (pQNode)malloc(sizeof(sQNode));
    char *abs_path = (char *)malloc(10000 * sizeof(char));

    abs_path = realpath(path, NULL);
    hisPtr = History, hisNode->path = strdup(abs_path);
    hisNode->Next = NULL;
    if (History == NULL) {
        History = hisNode;
        return;
    }
    while (hisPtr->Next) hisPtr = hisPtr->Next;
    hisPtr->Next = hisNode;
    return;
}

int32_t check_history(char *path) {
    pQNode hisPtr = History;
    int32_t idx = 0;
    char *abs_path = (char *)malloc(10000 * sizeof(char));

    abs_path = realpath(path, NULL);
    if (hisPtr == NULL) return -1;
    do {
        if (strcmp(abs_path, hisPtr->path) == 0) return idx;
    } while ((hisPtr = hisPtr->Next) != NULL && ++idx);
    return -1;
}
