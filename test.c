#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"


int main() {

    BMPImage bmp;
    char *fileName = "./bottom.bmp";
    int status = readBMPImage(fileName, &bmp);
    if (status != 0) {
        printf("Status: %d\n", status);
        return -1;
    }


    return 0;
}
