#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define SQR "\xe2\x96\xa0"

const uint32_t separator = 0xEE77;

int decode(uint8_t** ptr, size_t* size, size_t* width, FILE* desc);
int draw(uint8_t* ptr, size_t size, size_t width);

int main(){


    FILE* fptr = fopen("hexData", "rb");
    uint8_t* arr = nullptr;
    size_t size = 0;
    size_t width = 0;

    decode(&arr, &size, &width, fptr);
    printf("size: %d; width: %d\n", size, width);
    printf("data: ");
    for(int i = 0; i < size; i++){
        printf("%x", arr[i]);
    }
    

    draw(arr, size, 11);

    fclose(fptr);
    free(arr);
}

int draw(uint8_t* ptr, size_t size, size_t width){
    
    int byteCounter = 0;

    printf("\n");

    for(; byteCounter < size; byteCounter++){


        for(int i = 1; i <= 8; i++){
            if( (byteCounter * 8 + i - 1) % width == 0 ) printf("\n");
            if( (byteCounter * 8 + i) > (size * 8 - (size * 8) % width) ) return 0;
            if( ((ptr[byteCounter] & (1 << (8 - i))) >> (8 - i)) == 0 ) printf("\033[107m  \e[0m");
            else printf("\033[45m  \e[0m");
        }



    }



    printf("\n");
    return 0;
}


int decode(uint8_t** ptr, size_t* size, size_t* width, FILE* desc){
    uint32_t sepCheck = 0;
    size_t localSize = 0;

    fread(&localSize, sizeof(size), 1, desc);
    fread(&sepCheck, 2, 1, desc);

    if( sepCheck != separator ){
        printf("Error when parsing data size. Expected separator 0x%X, got 0x%X\n", sepCheck, separator);
        return 1;
    }

    *size = localSize;

    fread(width, sizeof(width), 1, desc);
    fread(&sepCheck, 2, 1, desc);
    if( sepCheck != separator ){
        printf("Error when parsing width. Expected separator 0x%X, got 0x%X\n", sepCheck, separator);
        return 1;
    }
    
    *ptr = (uint8_t*) calloc(1, localSize);
    fread(*ptr, localSize, 1, desc);

    fread(&sepCheck, 2, 1, desc);
    if( sepCheck != separator ){
        printf("Error when parsing width. Expected separator 0x%X, got 0x%X\n", sepCheck, separator);
        return 1;
    }

    return 0;
}
