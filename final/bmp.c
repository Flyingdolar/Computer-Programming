#include "bmp.h"

sbPict **initArr(int w, int h) {
    sbPict **arr = (sbPict **)malloc(h * sizeof(sbPict *));
    for (size_t hdx = 0; hdx < (size_t)h; hdx++)
        arr[hdx] = (sbPict *)malloc(w * sizeof(sbPict));
    return arr;
}

void freeArr(sbPict **arr, int h) {
    for (size_t hdx = 0; hdx < (size_t)h; hdx++)
        free(arr[hdx]);
    free(arr);
}

sbPict **BMPtoArr(FILE *bmp, sbHead *bmpHead) {
    int sizeW, sizeH;
    sbPict **arr;

    if ((fread(bmpHead, sizeof(*bmpHead), 1, bmp)) != 1) {
        PRINT_E("Read BMP Header Error");
        return NULL;
    }
    sizeW = bmpHead->w, sizeH = bmpHead->h;
    arr = initArr(sizeW, sizeH);
    fseek(bmp, bmpHead->off, SEEK_SET);

    for (size_t hdx = 0; hdx < (size_t)sizeH; hdx++) {
        for (size_t wdx = 0; wdx < (size_t)sizeW; wdx++) {
            if ((fread(&(arr[hdx][wdx]), 1, 3, bmp) != 3)) {
                PRINT_E("Read BMP Body Error");
                return NULL;
            }
        }
    }
    return arr;
}

int ArrtoBMP(FILE **bmp, sbPict **arr, sbHead bmpHead) {
    int sizeW = bmpHead.w, sizeH = bmpHead.h;

    if ((fwrite(&bmpHead, sizeof(bmpHead), 1, *bmp)) != 1) {
        PRINT_E("Write BMP Header Error");
        return -1;
    }
    fseek(*bmp, bmpHead.off, SEEK_SET);
    for (size_t hdx = 0; hdx < (size_t)sizeH; hdx++) {
        for (size_t wdx = 0; wdx < (size_t)sizeW; wdx++) {
            if ((fwrite(&arr[hdx][wdx], 1, 3, *bmp) != 3)) {
                PRINT_E("Write BMP Body Error");
                return -1;
            }
        }
    }
    return 0;
}
