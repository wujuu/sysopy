
#ifndef DBLOCKS_H
#define DBLOCKS_H

#define MAX_BLOCKS_SIZE 100
#define MAX_CHAR_ARRAY_SIZE 100

typedef struct charBlock{
    char* chars;
    int charsSize;
} charBlock;


typedef struct charBlockArray{
    charBlock** blocks;
    int blocksSize;
} charBlockArray;


void addBlock(charBlockArray* blocks, int index, int charsSize, char* chars);
void delBlock(charBlockArray* blocks, int index);
charBlockArray* initCharBlockArray(int blocksSize, int charsSize);
void delCharBlockArray(charBlockArray* blocks);
charBlockArray* makeRandCharBlockArray(int blocksSize, int charsSize);
//charBlockArray* makeUserCharBlockArray(int maxCharsSize);
int getSumOfChars(charBlock* chars);
charBlock* getMinBlock(charBlockArray* blocks, int closestTo);
void printCharBlockArray(charBlockArray* blocks);


#endif