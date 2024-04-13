#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#include "bmp.h"

bool fileExists(char *fileName) {
    return (access(fileName, F_OK) == 0);
}

bool verifyFileExtension(char *fileName) {
    char *fileExtension = fileName + (strlen(fileName) - 4);
    return (strcmp(fileExtension, ".bmp") == 0);
}

bool verifyFileSignature(char signature[]) {
    return (signature[0] == 'B' && signature[1] == 'M');
}

int32_t convertByteToInt(uint8_t *bytes, int byteNum) {
    int32_t result = 0;
    for (int i = 0; i < byteNum; i++) {
        result |= (int32_t)bytes[i] << (8 * i);
    }
    return result;
}

uint8_t *convertIntToByte(int num) {
    static uint8_t bytes[4];
    bytes[0] = num & 0xFF;
    bytes[1] = (num >> 8) & 0xFF;
    bytes[2] = (num >> 16) & 0xFF;
    bytes[3] = (num >> 24) & 0xFF;

    return bytes;
}

int32_t getBMPHeight(BMPInfoHeader *infoHeader) {
    return convertByteToInt(infoHeader->Height, 4);
}

int32_t getBMPWidth(BMPInfoHeader *infoHeader) {
    return convertByteToInt(infoHeader->Width, 4);
}

void cleanupPixels(Pixel **pixels, BMPInfoHeader *infoHeader) {
    int32_t height = getBMPHeight(infoHeader);
    for (int i = 0; i < height; i++) {
        if (pixels[i] != NULL) {
            free(pixels[i]);
            pixels[i] = NULL;
        }
    }
    free(pixels);
    pixels = NULL;
    return;
}

void cleanupBMPImage(BMPImage *image) {
    cleanupPixels(image->Pixels, &image->InfoHeader);
    free(image);
    image = NULL;
    return;
}

void printBMPInfo(char *fileName, BMPImage *image) {
    printf("File name: %s\n", fileName);
    printf("File size; %d\n", convertByteToInt(image->Header.FileSize, 4));
    printf("Width: %d\n", getBMPWidth(&image->InfoHeader));
    printf("Height: %d\n", getBMPHeight(&image->InfoHeader));
}

void printHeader(BMPHeader *header) {
    printf("Signature: %c%c\n", header->Signature[0], header->Signature[1]);
    printf("File Size: %d\n", convertByteToInt(header->FileSize, 4));
    printf("Reserved: %d\n", convertByteToInt(header->reserved, 4));
    printf("Offset: %d\n", convertByteToInt(header->Offset, 4));
}

void printInfoHeader(BMPInfoHeader *infoHeader) {
    printf("Header Size: %d\n", convertByteToInt(infoHeader->HeaderSize, 4));
    printf("Width: %d\n", getBMPWidth(infoHeader));
    printf("Height: %d\n", getBMPHeight(infoHeader));
}

void printPixels(Pixel **pixels, BMPInfoHeader *infoHeader) {
    int32_t height = getBMPHeight(infoHeader);
    int32_t width = getBMPWidth(infoHeader);

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            printf("%d ", pixels[i][j]);
        }
    }
}

int readBMPHeader(FILE *file, BMPHeader *header) {
    if (fread(header, sizeof(BMPHeader), 1, file) != 1) {
        puts("Failed to read file signature");
        return -1;
    }

    if (!verifyFileSignature(header->Signature)) {
        puts("Failed to verify file signature");
        return -2;
    }

    return 0;
}

int readBMPInfoHeader(FILE *file, BMPInfoHeader *infoHeader) {
    if (fread(infoHeader, sizeof(BMPInfoHeader), 1, file) != 1) {
        puts("Failed to read BMP Info Header");
            return -1;
    }

    return 0;
}

int readBMPPixels(FILE *file, BMPInfoHeader *infoHeader, Pixel **pixels) {
    int32_t height = getBMPHeight(infoHeader);
    int32_t width = getBMPWidth(infoHeader);

    int32_t bitsPerPixel = convertByteToInt(infoHeader->BitPerPixel, 2);
    int32_t padding = (4 - ((width * bitsPerPixel) % 4)) % 4;

    printf("BitsPerPixel: %d\nPadding: %d\n", bitsPerPixel, padding);

    Pixel pixel;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (fread(&pixel, sizeof(Pixel), 1, file) != 1) {
                puts("Failed to read pixel");
                return -1;
            }

            printf("Pixel (%d, %d)B: 0x%02X, G: 0x%02X, R: 0x%02X\n",
                   i, j, pixel.Blue, pixel.Green, pixel.Red);
        }
        fseek(file, padding, SEEK_CUR);
    }

    return 0;
}



int readBMPImage(char *fileName, BMPImage *bmp) {
    if (!verifyFileExtension(fileName)) {
        printf("Provided file name %s does not end with .bmp\n", fileName);
        return -1;
    }

    if (!fileExists(fileName)) {
        printf("Cannot locate file with name %s\n", fileName);
        return -2;
    }

    FILE *file = fopen(fileName, "rb");
    if (file == NULL) {
        printf("Cannot open file with name %s\n", fileName);
        return -3;
    }

int readHeaderStatus = readBMPHeader(file, &bmp->Header);
    if (readHeaderStatus != 0) {
        return -1;
    }
    int readInfoHeaderStatus = readBMPInfoHeader(file, &bmp->InfoHeader);
    if (readInfoHeaderStatus != 0) {
        return -1;
    }

    printBMPInfo(fileName, bmp);

    puts("\n=========================== READ PIXELS ===========================\n");

    Pixel **pixels;

    int readPixelDataStatus = readBMPPixels(file, &bmp->InfoHeader, pixels);
    if (readPixelDataStatus != 0) {
        return -1;
    }
    return 0;
}
