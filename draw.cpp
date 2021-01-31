#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATAERR 1

canvas::canvas(FILE* desc):
    ptr(nullptr),
    size(0),
    width(0) {

    uint32_t sepCheck = 0;
    size_t localSize = 0;

    fread(&localSize, sizeof(this -> size), 1, desc);
    fread(&sepCheck, 2, 1, desc);

    if( sepCheck != this -> separator ){
        printf("Error when parsing data size. Expected separator 0x%X, got 0x%X\n", this -> separator, sepCheck);
        throw DATAERR;
    }

    this -> size = localSize;

    fread(&(this -> width), sizeof(this -> width), 1, desc);
    fread(&sepCheck, 2, 1, desc);
    if( sepCheck != this -> separator ){
        printf("Error when parsing width. Expected separator 0x%X, got 0x%X\n", this -> separator, sepCheck);
        throw DATAERR;
    }
    
    this -> ptr = (uint8_t*) calloc(1, localSize);
    fread(this -> ptr, localSize, 1, desc);

    fread(&sepCheck, 2, 1, desc);
    if( sepCheck != this -> separator ){
        printf("Error when parsing width. Expected separator 0x%X, got 0x%X\n", this -> separator, sepCheck);
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
   fwrite(&(this -> separator), 2, 1, desc);
   fwrite(&(this -> width), sizeof(this -> width), 1, desc);
   fwrite(&(this -> separator), 2, 1, desc);
   fwrite(ptr, this -> size, 1, desc);
   fwrite(&(this -> separator), 2, 1, desc);

    return 0;
}

int canvas::togglePixel(unsigned int x, unsigned int y){
    if(getPixel(x, y) == 1) unsetPixel(x, y);
    else setPixel(x, y);

    return 0;
}

int canvas::setState(unsigned int x, unsigned int y, unsigned int state){
    if( (state & 1) == 1 ) setPixel(x, y);
    else unsetPixel(x, y);

    return 0;
}

int canvas::contDisplay(){
    system("oldPS=\"$PS1\"");
    system("PS1=\'\'");
    char command[20] = "";
    char stop[5] = "exit";
    unsigned int x = 0;
    unsigned int y = 0;
    unsigned int state = 0;

    while( strcmp(command, stop) != 0 ){
        system("clear");
        draw();
        printf("\n\n\n\n\n\n\n\n\n\n");
        printf("--------------------------------------------------------\n");
        printf("> ");
        scanf("%s", command);
        
        if( strcmp(command, "help") == 0 ){
            printf("After output of a command press ENTER again\n"
                   "help: summon this help\n"
                   "exit: stop the editor\n"
                   "setPixel x y: sets pixel (x, y) to 1\n"
                   "unsetPixel x y: sets pixel (x, y) to 0\n"
                   "setState state x y: sets pixel (x, y) to state\n"
                   "togglePixel x y: toggle the state of pixel (x, y)\n"
                   "save filename: after a confirmation saves current canvas to ./filename\n"
                   "load filename: after a confirmation loads canvas from filename\n");
        }

        else if( strcmp(command, "setPixel") == 0 ){
                 scanf("%d %d", &x, &y);
                 setPixel(x, y);
                 printf("Set pixel (%d, %d) to 1\n", x, y);
        }

        else if( strcmp(command, "unsetPixel") == 0 ){
                 scanf("%d %d", &x, &y);
                 unsetPixel(x, y);
                 printf("Set pixel (%d, %d) to 0\n", x, y);
        }

        else if( strcmp(command, "togglePixel") == 0 ){
                 scanf("%d %d", &x, &y);
                 togglePixel(x, y);
                 printf("Toggled pixel (%d, %d)\n", x, y);
        }

        else if( strcmp(command, "setState") == 0 ){
                 scanf("%d %d %d", state, x, y);
                 setState(state, x, y);
                 printf("Set pixel (%d, %d) to %d\n", x, y, state);
        }

        else if( strcmp(command, "save") == 0 ){
                 printf("Enter filename\n> ");
                 char name[20] = "";
                 scanf("%s", name);
                 printf("Filename was read as: %s. Do you want to save to it? (y/n): ", name);
                 scanf("%s", command);
                 if( strcmp(command, "y") == 0 ){
                     FILE* fptr = fopen(name, "wb");
                     save(fptr);
                     fclose(fptr);
                 }
        }

        else if( strcmp(command, "load") == 0 ){
                 printf("Enter filename\n> ");
                 char name[20] = "";
                 scanf("%s", name);
                 printf("Filename was read as: %s. Do you want to read from it? (y/n): ", name);
                 scanf("%s", command);
                 if( strcmp(command, "y") == 0 ){
                     FILE* fptr = fopen(name, "rb");
                     load(fptr);
                     fclose(fptr);
                 }

        }

        while( getc(stdin) != '\n' );
        while( getc(stdin) != '\n' );


    }

    system("PS1=\"$oldPS\"");

    return 0;
}

int canvas::load(FILE* desc){
    uint32_t sepCheck = 0;
    size_t localSize = 0;

    fread(&localSize, sizeof(this -> size), 1, desc);
    fread(&sepCheck, 2, 1, desc);

    if( sepCheck != this -> separator ){
        printf("Error when parsing data size. Expected separator 0x%X, got 0x%X\n", this -> separator, sepCheck);
        throw DATAERR;
    }

    this -> size = localSize;

    fread(&(this -> width), sizeof(this -> width), 1, desc);
    fread(&sepCheck, 2, 1, desc);
    if( sepCheck != this -> separator ){
        printf("Error when parsing width. Expected separator 0x%X, got 0x%X\n", this -> separator, sepCheck);
        throw DATAERR;
    }
    
    this -> ptr = (uint8_t*) calloc(1, localSize);
    fread(this -> ptr, localSize, 1, desc);

    fread(&sepCheck, 2, 1, desc);
    if( sepCheck != this -> separator ){
        printf("Error when parsing width. Expected separator 0x%X, got 0x%X\n", this -> separator, sepCheck);
        throw DATAERR;
    }




    return 0;
}




