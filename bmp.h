#pragma once
#ifndef _BMP_H
#define _BMP_H

#include <stdint.h>

typedef struct {
    char Signature[2]; // should be 2 "BM"
    uint8_t FileSize[4];
    uint8_t reserved[4];
    uint8_t Offset[4];
} BMPHeader;

typedef struct {
    uint8_t HeaderSize[4];
    uint8_t Height[4];
    uint8_t Width[4];
    uint8_t Planes[2];
    uint8_t BitPerPixel[2];
    uint8_t Compression[4]; // 0 = BI_RGB, 1 = BI_RLE8, 2 = BI_RLE4
    uint8_t ImageSize[4]; // 0 if Compression == 0
    uint8_t XPixelsPerMeter[4];
    uint8_t YPixelsPerMeter[4];
    uint8_t numColors[4];
    uint8_t ImportantColors[4];
} BMPInfoHeader;

typedef struct {
    uint8_t Blue;
    uint8_t Green;
    uint8_t Red;
} Pixel;

typedef struct {
    BMPHeader Header;
    BMPInfoHeader InfoHeader;
    Pixel **Pixels;
} BMPImage;

int readBMPImage(char *fileName, BMPImage *bmp);

int32_t getBMPHeight(BMPInfoHeader *infoHeader);
int32_t getBMPWidth(BMPInfoHeader *infoHeader);

#endif
