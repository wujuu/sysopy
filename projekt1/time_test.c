#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dblocks.h"
#include "sblocks.h"






int main(int argc, char** argv){
    //10 10 static 2000, 6, 6;

    int blocksSize = atoi(argv[1]);
    int charsSize = atoi(argv[2]);
    char* alloc_type = argv[3]; //static / dynamic
    int* test_atr = calloc(3, sizeof(int));

    const char* dynamic = "dynamic";

    char* tmp = calloc(charsSize, sizeof(char));

    for(int j = 0; j < charsSize; j++){
        tmp[j] = 32 + rand() % 95;
    }


     for(int i = 0; i < 3; i++){
         test_atr[i] = atoi(argv[i + 4]);
     }
     

    if(strcmp(alloc_type, dynamic) == 0){

        //measure time
        charBlockArray* dynamicArray = initCharBlockArray(blocksSize, charsSize);
        //end measure

        //NOT MEASURING for testing's sake;
        dynamicArray = makeRandCharBlockArray(blocksSize, charsSize);

        //measure time
        getMinBlock(dynamicArray, test_atr[0]);
        //end measure

        int i = 0;     

        //measure time
        while(i < test_atr[1]){
            delBlock(dynamicArray, i);
            i = (i+1) % blocksSize;
        }

        while(i < test_atr[1]){
            addBlock(dynamicArray, i, charsSize, tmp);
            i = (i+1) % blocksSize;
        }
        //end measure

        i = 0;

        //start measure
        while(i < test_atr[2]){
            delBlock(dynamicArray, i);
            addBlock(dynamicArray, i, charsSize, tmp);
            i = (i+1) % blocksSize;
        }
        //end measure
    }
    else{
        //measure time
        staticCharBlockArray staticArray = staticInitCharBlockArray(blocksSize, charsSize);
        //end measure

        //for testing's sake;
        staticArray = staticMakeRandCharBlockArray(blocksSize, charsSize);

        //measure time
        staticGetMinIndex(staticArray, test_atr[0]);
        //end measure

        int i = 0;     

        //measure time
        while(i < test_atr[1]){
            staticDelBlock(&staticArray, i);
            i = (i+1) % blocksSize;
        }

        while(i < test_atr[1]){
            staticAddBlock(&staticArray, i, charsSize, tmp);
            i = (i+1) % blocksSize;
        }
        //end measure

        i = 0;

        //start measure
        while(i < test_atr[2]){
            staticDelBlock(&staticArray, i);
            staticAddBlock(&staticArray, i, charsSize, tmp);
            i = (i+1) % blocksSize;
        }
        //end measure

    }



        

    return 0;
}