#pragma once
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _sBmpHeader sbHead;
struct _sBmpHeader {
    char bm[2];
    uint32_t size;      // File Size (Bytes)
    uint32_t rs;        // Reserve: Mostly is 0
    uint32_t off;       // Offset (Size of the content under -> 40)
    uint32_t bSize;     // Header Size (Bitmap -> 40)
    int32_t w;          // Image Width
    int32_t h;          // Image Height
    uint16_t pl;        // Image Plane (normally is 1)
    uint16_t bpp;       // Bit per Pixels -> 1 2 4 ...
    uint32_t cpress;    // Compression (0 means no compression)
    uint32_t bImgSize;  // Bitmap Size
    int32_t Xres;       // Horizontal Resolution (Pixel/meter)
    int32_t Yres;       // Vertical Resolution (Pixel/meter)
    uint32_t used;      // Used Color (0 means ALL)
    uint32_t impt;      // Important Color (0 means ALL)
} __attribute__((__packed__));

typedef struct _sBmp8Body sbPict;
struct _sBmp8Body {
    uint8_t b;
    uint8_t g;
    uint8_t r;
} __attribute__((__packed__));

typedef struct _pos_ {
    int x, y;
    int w, h;
    int num;
} sPos;

// Array function
sbPict **initArr(int w, int h);
void freeArr(sbPict **arr, int h);

// Transform function
sbPict **BMPtoArr(FILE *bmp, sbHead *bmpHead);
int ArrtoBMP(FILE **bmp, sbPict **arr, sbHead bmpHead);

// Modify function
int drawMosaic(sbPict **arr, sbHead bmpHead, sPos pos);
