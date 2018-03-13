#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "dblocks.h"


void addBlock(charBlockArray* blocks, int index, int charsSize, char* chars){

    char* tmp = calloc(charsSize, sizeof(char));

    for(int j = 0; j < charsSize; j++){
        tmp[j] = chars[j];
    }

    blocks -> blocks[index] -> chars = tmp;

    blocks -> blocks[index] -> charsSize = charsSize;
}


void delBlock(charBlockArray* blocks, int index){

    free(blocks -> blocks[index] -> chars);

    blocks -> blocks[index] -> charsSize = 0;

    //free(blocks -> blocks[index]);
}


charBlockArray* initCharBlockArray(int blocksSize, int charsSize){

    srand(time(NULL));

    charBlockArray* blocks = calloc(1, sizeof(charBlockArray));

    blocks -> blocksSize = blocksSize;   

    blocks -> blocks = calloc(blocksSize, sizeof(charBlock*));

    for(int i = 0; i < blocksSize; i++){
        blocks -> blocks[i] = calloc(1, sizeof(charBlock));
        blocks -> blocks[i] -> charsSize = charsSize;
    }

    return blocks;
}


void delCharBlockArray(charBlockArray* blocks){

    for(int i = 0; i < blocks -> blocksSize; i++){
        delBlock(blocks, i);
    }

    free(blocks);
}


charBlockArray* makeRandCharBlockArray(int blocksSize, int charsSize){

    srand(time(NULL));

    charBlockArray* blocks = initCharBlockArray(blocksSize, charsSize);

    blocks -> blocksSize = blocksSize;

    for(int i = 0; i < blocks -> blocksSize; i++){

        char* tmpChars = calloc(charsSize, sizeof(char));

        for(int j = 0; j< charsSize; j++){
            tmpChars[j] = 32 + rand() % 95;
        }

        addBlock(blocks, i, charsSize, tmpChars);
    }

    return blocks;
}


//additional function for debugging
// charBlockArray* makeUserCharBlockArray(int maxCharsSize){

//     int blocksSize;

//     char* charBlocksSize = calloc(4, sizeof(char));

//     printf("Input charBlockArray size: ");

//     int* fresult = fgets(charBlocksSize, 4, stdin);

//     blocksSize = atoi(charBlocksSize);

//     printf("\n");

//     charBlockArray* blocks = initCharBlockArray(blocksSize);
    
//     for(int i = 0; i < blocksSize; i++){

//         char* maxChars = calloc(maxCharsSize, sizeof(char));

//         printf("Input %i-th block chars: ", i);

//         int* fresult = fgets(maxChars, maxCharsSize, stdin);

//         int currCharsSize = 0, j = 0; 

//         while(maxChars[j] != '\0' && j < maxCharsSize){
//             currCharsSize++;
//             j++;
//         }

//         char* currChars = calloc(currCharsSize - 1, sizeof(char));

//         for(int j = 0; j < currCharsSize; j++){
//             currChars[j] = maxChars[j];
//         }

//         free(maxChars); 

//         addBlock(blocks, i, currCharsSize, currChars);    
//     }

//     printf("\n");

//     return blocks;
//}


int getSumOfChars(charBlock* chars){
    int sum = 0;

    for(int i = 0; i < chars -> charsSize; i++){
        sum += chars -> chars[i];
    }

    return sum;
}


charBlock* getMinBlock(charBlockArray* blocks, int closestTo){
    
    int minDist = abs(getSumOfChars(blocks -> blocks[0]) - closestTo);

    charBlock* minBlock = blocks -> blocks[0];

    for(int i = 1; i < blocks -> blocksSize; i++){
        int currDist = abs(getSumOfChars(blocks -> blocks[i]) - closestTo);

        if(currDist < minDist){ 
            minDist = currDist; 
            minBlock = blocks -> blocks[i];
        
        }
    }

    return minBlock;
}


void printCharBlockArray(charBlockArray* blocks){
    for(int i = 0; i < blocks -> blocksSize; i++){

        for(int j = 0; j < blocks -> blocks[i] -> charsSize; j++){
            printf("%c", blocks -> blocks[i] -> chars[j]);
        }

        printf("\n");
    }
}  