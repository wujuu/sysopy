#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sblocks.h"


void staticAddBlock(staticCharBlockArray *blocks, int index, int charsSize, char chars[]){

    for(int i = 0; i < charsSize && i < STATIC_MAX_CHAR_ARRAY_SIZE; i++){
        blocks -> array[index][i] = chars[i];
    }

    blocks -> charsSize[index] = charsSize;
} 


void staticDelBlock(staticCharBlockArray *blocks, int index){
    for(int i = 0; i < blocks -> charsSize[index]; i++){
        blocks -> array[index][i] = ' ';
    }

    blocks -> charsSize[index] = 0;
}


staticCharBlockArray staticInitCharBlockArray(int blocksSize, int charsSize){
    staticCharBlockArray blocks;

    blocks.blocksSize = blocksSize;

    for(int i = 0; i < blocksSize; i++){
        blocks.charsSize[i] = charsSize;
    }
    

    return blocks;
}

void staticDelCharBlockArray(staticCharBlockArray *blocks){

    for(int i = 0; i < blocks -> blocksSize; i++){
        blocks -> charsSize[i] = 0;
    }

    blocks -> blocksSize = 0;
}


staticCharBlockArray staticMakeRandCharBlockArray(int blocksSize, int charsSize){
    srand(time(NULL));

    staticCharBlockArray blocks;

    blocks.blocksSize = blocksSize;

    for(int i = 0; i < blocks.blocksSize; i++){

        int currCharsSize = charsSize;

        blocks.charsSize[i] = currCharsSize;

        for(int j = 0; j < blocks.charsSize[i]; j++){

            blocks.array[i][j] = 32 + rand() % 95;

        }
    }

    return blocks;
}



int staticGetSumOfChars(staticCharBlockArray blocks, int index){
    int sum = 0;

    for(int i = 0; i < blocks.charsSize[index]; i++){
        sum += blocks.array[index][i];
    }

    return sum;
}

int staticGetMinIndex(staticCharBlockArray blocks, int closestTo){

    int minDist = abs(staticGetSumOfChars(blocks, 0) - closestTo);

    int minIndex = 0;

    for(int i = 1; i < blocks.blocksSize; i++){
        int currDist = abs(staticGetSumOfChars(blocks, i) - closestTo);

        if(currDist < minDist){ 
            minDist = currDist; 
            minIndex = i;
        
        }
    }

    return minIndex;
}

void staticPrintCharBlockArray(staticCharBlockArray blocks){
    for(int i = 0; i < blocks.blocksSize; i++){
        for(int j = 0; j < blocks.charsSize[i]; j++){
            printf("%c", blocks.array[i][j]);
        }

        printf("\n");
    }

}