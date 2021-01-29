#pragma once
#include <stdio.h>
#include <stdlib.h>

#define DATAERR 1

canvas::canvas(FILE* desc):
    ptr(nullptr),
    size(0),
    width(0) {

    uint32_t sepCheck = 0;
    size_t localSize = 0;

    fread(&localSize, sizeof(this -> size), 1, desc);
    fread(&sepCheck, 2, 1, desc);

    if( sepCheck != separator ){
        printf("Error when parsing data size. Expected separator 0x%X, got 0x%X\n", sepCheck, separator);
        throw DATAERR;
    }

    this -> size = localSize;

    fread(&(this -> width), sizeof(this -> width), 1, desc);
    fread(&sepCheck, 2, 1, desc);
    if( sepCheck != separator ){
        printf("Error when parsing width. Expected separator 0x%X, got 0x%X\n", sepCheck, separator);
        throw DATAERR;
    }
    
    this -> ptr = (uint8_t*) calloc(1, localSize);
    fread(this -> ptr, localSize, 1, desc);

    fread(&sepCheck, 2, 1, desc);
    if( sepCheck != separator ){
        printf("Error when parsing width. Expected separator 0x%X, got 0x%X\n", sepCheck, separator);
        throw DATAERR;
    }
}

canvas::~canvas(){
    free(this -> ptr);
}

int canvas::draw(){
    
    int byteCounter = 0;

    for(; byteCounter < this -> size; byteCounter++){
        for(int i = 1; i <= 8; i++){
            if( ((byteCounter * 8 + i - 1) % this -> width == 0) && (byteCounter * 8 + i - 1 != 0) ) printf("\n");
            if( (byteCounter * 8 + i) > (this -> size * 8 - (this -> size * 8) % this -> width) ) return 0;
            if( ((this -> ptr)[byteCounter] & (1 << (8 - i))) >> (8 - i) == 0 ) printf("\033[107m  \e[0m");
            else printf("\033[45m  \e[0m");
        }
    }



    printf("\e[0m\n");
    return 0;
}

canvas::canvas(size_t _size, size_t _width):
    ptr(nullptr),
    size(_size),
    width(_width) {
    ptr = (uint8_t*) calloc(_size, 1);
}

int canvas::setPixel(unsigned int x, unsigned int y){
    
    unsigned int offset = y * (this -> width) + x;
    this -> ptr [(int) offset / 8] |= (1 << (7 - (int) (offset % 8)));
    
    return 0;
}

int canvas::unsetPixel(unsigned int x, unsigned int y){
    
    unsigned int offset = y * (this -> width) + x;
    this -> ptr [(int) offset / 8] &= (~(1 << (7 - (int) (offset % 8))));
    
    return 0;
}

int canvas::getPixel(unsigned int x, unsigned int y){

  unsigned int offset = y * (this -> width) + x;
  
  return ((this -> ptr [(int) offset / 8] & (1 << (7 - (int) (offset % 8)))) >> (7 - (int) (offset % 8)));



}

int canvas::save(FILE* desc){
   fwrite(&(this -> size), sizeof(this -> size), 1, desc);
   fwrite(&separator, 2, 1, desc);
   fwrite(&(this -> width), sizeof(this -> width), 1, desc);
   fwrite(&separator, 2, 1, desc);
   fwrite(ptr, this -> size, 1, desc);
   fwrite(&separator, 2, 1, desc);

    return 0;
}





