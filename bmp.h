#pragma once
#ifndef _BMP_H
#define _BMP_H

#include <stdint.h>

typedef struct {
    char Signature[3];
    uint32_t FileSize;
    uint32_t reserved;
    uint32_t Offset;
} BMPHeader;

typedef struct {
    uint32_t Size;
    uint32_t Width;
    uint32_t Height;
    uint16_t Planes;
    uint16_t BytesPerPixel;
    uint32_t Compression; // 0 = BI_RGB, 1 = BI_RLE8, 2 = BI_RLE4
    uint32_t ImageSize; // 0 if Compression == 0
    uint32_t XPixelsPerMeter;
    uint32_t YPixelsPerMeter;
    uint32_t ImportantColors;
} BMPInfoHeader;

typedef struct {
    uint8_t Red;
    uint8_t Green;
    uint8_t Blue;
    uint8_t reserved;
} Pixel;

typedef struct {
    BMPHeader Header;
    BMPInfoHeader InfoHeader;
    Pixel **Pixels;
} BMPImage;



int readBMPImage(char *fileName, BMPImage bmp);

int32_t getImageHeight(BMPHeader header);
int32_t getImageWidth(BMPHeader header);


#endif
