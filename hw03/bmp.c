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

int drawMosaic(sbPict **arr, sbHead bmpHead, sPos pos) {
    pos.y = (bmpHead.h - 1) - pos.y;
    pos.w += pos.x, pos.h = pos.y - pos.h;
    if (pos.x < 0 || pos.y < 0 || pos.x >= bmpHead.w || pos.y >= bmpHead.h) {
        PRINT_E("Position Error: Please check X, Y value is valid.");
        return -1;
    }
    if (pos.w < 0 || pos.h < 0 || pos.w >= bmpHead.w || pos.h >= bmpHead.h) {
        PRINT_E("Size Error: Please check W, H value is valid.");
        return -1;
    }

    for (int hdx = pos.y; hdx > pos.h; hdx -= pos.num) {
        for (int wdx = pos.x; wdx < pos.w; wdx += pos.num) {
            int r = 0, g = 0, b = 0, count = 0;
            for (int h = hdx; h > hdx - pos.num && h > pos.h; h--) {
                for (int w = wdx; w < wdx + pos.num && w < pos.w; w++) {
                    r += arr[h][w].r;
                    g += arr[h][w].g;
                    b += arr[h][w].b;
                    count++;
                }
            }
            r /= count, g /= count, b /= count;
            for (int h = hdx; h > hdx - pos.num && h > pos.h; h--) {
                for (int w = wdx; w < wdx + pos.num && w < pos.w; w++) {
                    arr[h][w].r = r;
                    arr[h][w].g = g;
                    arr[h][w].b = b;
                }
            }
        }
    }
    return 0;
}
