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

uint32_t convertByteToInt(uint8_t *bytes, int byteNum) {
    uint32_t result = 0;
    for (int i = 0; i < byteNum; i++) {
        result |= (int32_t)bytes[i] << (8 * i);
    }
    return result;
}

void printHeader(BMPHeader *header) {
    printf("Signature: %c%c\n", header->Signature[0], header->Signature[1]);
    printf("File Size: %d\n", convertByteToInt(header->FileSize, 4));
    printf("Reserved: %d\n", convertByteToInt(header->reserved, 4));
    printf("Offset: %d\n", convertByteToInt(header->Offset, 4));
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

    int readHeaderStatus = readBMPHeader(file, &(bmp->Header));
    if (readHeaderStatus != 0) {
        return -1;
    }

    printHeader(&bmp->Header);
    return 0;
}
