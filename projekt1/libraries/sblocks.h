
#ifndef SBLOCKS_H
#define SBLOCKS_H

#define STATIC_MAX_BLOCKS_SIZE 100
#define STATIC_MAX_CHAR_ARRAY_SIZE 100

typedef struct staticCharBlockArray{
    char array[STATIC_MAX_BLOCKS_SIZE][STATIC_MAX_CHAR_ARRAY_SIZE];
    int charsSize[STATIC_MAX_BLOCKS_SIZE];
    int blocksSize;
} staticCharBlockArray;

void staticAddBlock(staticCharBlockArray *blocks, int index, int charsSize, char chars[]);
void staticDelBlock(staticCharBlockArray *blocks, int index);
staticCharBlockArray staticInitCharBlockArray(int blocksSize, int charsSize);
void staticDelCharBlockArray(staticCharBlockArray *blocks);
staticCharBlockArray staticMakeRandCharBlockArray(int blocksSize, int charsSize);
int staticGetSumOfChars(staticCharBlockArray blocks, int index);
int staticGetMinIndex(staticCharBlockArray blocks, int closestTo);
void staticPrintCharBlockArray(staticCharBlockArray blocks);

#endif
