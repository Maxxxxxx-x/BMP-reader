#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#include "bmp.h"

bool fileExists(char *fileName) {
    return (access(fileName, F_OK) == 0);
}

int readBMPImage(char *fileName, BMPImage bmp) {
    return 0;
}
