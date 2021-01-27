#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


const uint32_t separator = 0xEE77;

int toHexFile(void* src, size_t dataSize, size_t width, FILE* dest);
int toBitmapFile(void* src, size_t dataSize, size_t width, FILE* dest);
int printData(const void* ptr, size_t dataSize, FILE* dest);




int main(){


    uint8_t* graph = (uint8_t*) calloc(17, 1);

//fff00710c5191341682c898a308600fff  

/*
11111111111
11000000001
10000100001
10001010001
10010001001
10100000101
10010001001
10001010001
10000100001
10000000001
11111111111








*/

    graph[0] =  0xff;
    graph[1] =  0xf0;
    graph[2] =  0x07;
    graph[3] =  0x10;
    graph[4] =  0xc5;
    graph[5] =  0x19;
    graph[6] =  0x13;
    graph[7] =  0x41;
    graph[8] =  0x68;
    graph[9] =  0x2c;
    graph[10] = 0x89;
    graph[11] = 0x8a;
    graph[12] = 0x30;
    graph[13] = 0x86;
    graph[14] = 0x00;
    graph[15] = 0xff;
    graph[16] = 0xf0;



    FILE* fptr = fopen("hexData", "wb");
    toHexFile(graph, 17, 11, fptr);

    fclose(fptr);
    free(graph);

    return 0;
}

int toHexFile(void* src, size_t dataSize, size_t width, FILE* dest){
    fwrite(&dataSize, sizeof(dataSize), 1, dest);
    fwrite(&separator, 2, 1, dest);
    fwrite(&width, sizeof(width), 1, dest);
    fwrite(&separator, 2, 1, dest);
    fwrite(src, dataSize, 1, dest);
    fwrite(&separator, 2, 1, dest);

    return 0;
}


int printData(const void* voidPtr, size_t dataSize, FILE* dest){
    const char* ptr = (const char*) voidPtr;
    for(size_t i = 0; i < dataSize; i++){
        if(ptr[i] != 0) fwrite(ptr + i, 1, 1, dest);
    }

    return 0;
}












