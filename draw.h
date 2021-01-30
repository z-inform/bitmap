#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define SQR "\xe2\x96\xa0"

const uint32_t separator = 0xEE77;

class canvas{
    uint8_t* ptr;
    size_t size;
    size_t width;
    
    public:
    canvas(FILE* desc);
    canvas(size_t _size, size_t width);
    ~canvas();

    int draw();
    int setPixel(unsigned int x, unsigned int y);
    int unsetPixel(unsigned int x, unsigned int y);
    int getPixel(unsigned int x, unsigned int y);
    int save(FILE* desc);
    int togglePixel(unsigned int x, unsigned int y);
    int setState(unsigned int x, unsigned int y, unsigned int state);



};


#include "draw.cpp"
