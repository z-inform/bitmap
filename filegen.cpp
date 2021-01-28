#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


const uint32_t separator = 0xEE77;

int toHexFile(void* src, size_t* dataSize, size_t* width, FILE* dest);
int toBitmapFile(void* src, size_t dataSize, size_t width, FILE* dest);
int printData(const void* ptr, size_t dataSize, FILE* dest);




int main(){


    uint8_t* graph = (uint8_t*) calloc(16, 1);

/*

11111111
11111000
00000110
00010000
11000101
00011001
00010011
01000001
01100100
01001100
01010001
10000100
00110000
00000111
11111111
10000000




121 bit
128 cap
16 byte


*/

    graph[0] =  0b11111111;
    graph[1] =  0b11111000;
    graph[2] =  0b00000110;
    graph[3] =  0b00010000;
    graph[4] =  0b11000101;
    graph[5] =  0b00011001;
    graph[6] =  0b00010011;
    graph[7] =  0b01000001;
    graph[8] =  0b01100100;
    graph[9] =  0b01001100;
    graph[10] = 0b01010001;
    graph[11] = 0b10000100;
    graph[12] = 0b00110000;
    graph[13] = 0b00000111;
    graph[14] = 0b11111111;
    graph[15] = 0b10000000;



    FILE* fptr = fopen("hexData", "wb");
    size_t size = 16;
    size_t width = 11;
    toHexFile(graph, &size, &width, fptr);

    fclose(fptr);
    free(graph);

    return 0;
}

int toHexFile(void* src, size_t* dataSize, size_t* width, FILE* dest){
    fwrite(dataSize, sizeof(*dataSize), 1, dest);
    fwrite(&separator, 2, 1, dest);
    fwrite(width, sizeof(*width), 1, dest);
    fwrite(&separator, 2, 1, dest);
    fwrite(src, *dataSize, 1, dest);
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












