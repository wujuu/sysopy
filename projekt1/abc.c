#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dblocks.h"
#include "sblocks.h"


int main(){
    int blocksSize = 10;
    int charsSize = 5;

    charBlockArray* dynamicArray = initCharBlockArray(blocksSize, charsSize);

    dynamicArray = makeRandCharBlockArray(blocksSize, charsSize);  

    printCharBlockArray(dynamicArray);

    char* tmp = calloc(20, sizeof(char));
    for(int j = 0; j < 20; j++){
        tmp[j] = 32 + rand() % 95;
    }

    

    delBlock(dynamicArray, 2);

    printf("\n");

    printCharBlockArray(dynamicArray);


    addBlock(dynamicArray, 2, 2, tmp);

    printf("\n");

    printCharBlockArray(dynamicArray);

     delBlock(dynamicArray, 2);

    printf("\n");

    printCharBlockArray(dynamicArray);


    addBlock(dynamicArray, 2, 2, tmp);

    printf("\n");

    printCharBlockArray(dynamicArray);


}