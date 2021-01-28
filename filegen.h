#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


const uint32_t separator = 0xEE77;

int toHexFile(void* src, size_t* dataSize, size_t* width, FILE* dest);
















int toHexFile(void* src, size_t* dataSize, size_t* width, FILE* dest){
    fwrite(dataSize, sizeof(*dataSize), 1, dest);
    fwrite(&separator, 2, 1, dest);
    fwrite(width, sizeof(*width), 1, dest);
    fwrite(&separator, 2, 1, dest);
    fwrite(src, *dataSize, 1, dest);
    fwrite(&separator, 2, 1, dest);

    return 0;
}
