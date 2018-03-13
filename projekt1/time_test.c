#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dblocks.h"
#include "sblocks.h"






int main(int argc, char** argv){
    //10 10 static 2000 6 1000;

    int blocksSize = atoi(argv[1]);
    int charsSize = atoi(argv[2]);
    char* alloc_type = argv[3]; //static / dynamic
    int closestTo = atoi(argv[4]);
    int times_add_del_cont = atoi(argv[5]) - 1;
    int times_add_del_mix = atoi(argv[6]) - 1;


    char* tmp = calloc(charsSize, sizeof(char));

    for(int j = 0; j < charsSize; j++){
        tmp[j] = 32 + rand() % 95;
    }

     
    const char* dynamic = "dynamic";

    if(strcmp(alloc_type, dynamic) == 0){

        //measure time
        charBlockArray* dynamicArray = initCharBlockArray(blocksSize, charsSize);
        //end measure
        printf("Done initializing..\n");

        //NOT MEASURING for testing's sake;
        dynamicArray = makeRandCharBlockArray(blocksSize, charsSize);

        //measure time
        getMinBlock(dynamicArray, closestTo);
        //end measure
        printf("Done finding closest block..\n");


        int i = 0;     

        //measure time
        while(i <= times_add_del_cont){
            delBlock(dynamicArray, i);
            i = (i+1);
            i = i % (blocksSize + 1);
        }
        printf("Done deleting..\n");


        i = 0;

        while(i <= times_add_del_cont){
            addBlock(dynamicArray, i, charsSize, tmp);
            i = (i+1);
            i = i % (blocksSize + 1);
        }
        //end measure
        printf("Done adding..\n");
        

        i = 0;

        // start measure
        while(i <= times_add_del_mix){
            delBlock(dynamicArray, i);
            addBlock(dynamicArray, i, charsSize, tmp);
            i = (i+1);
            i = i % (blocksSize + 1);
        }
        //end measure
        printf("Done deleting and adding..\n");
    }
    else{
        //measure time
        staticCharBlockArray staticArray = staticInitCharBlockArray(blocksSize, charsSize);
        //end measure

        //for testing's sake;
        staticArray = staticMakeRandCharBlockArray(blocksSize, charsSize);

        //measure time
        staticGetMinIndex(staticArray, closestTo);
        //end measure

        int i = 0;     

        //measure time
        while(i < times_add_del_cont){
            staticDelBlock(&staticArray, i);
            i = (i+1) % blocksSize;
        }

        i = 0;

        while(i <= times_add_del_cont){
            staticAddBlock(&staticArray, i, charsSize, tmp);
            i = (i+1);
            i = i % (blocksSize + 1);
        }
        //end measure

        i = 0;

        //start measure
        while(i <= times_add_del_mix){
            staticDelBlock(&staticArray, i);
            staticAddBlock(&staticArray, i, charsSize, tmp);
            i = (i+1);
            i = i % (blocksSize + 1);
        }
        //end measure

    }



        

    return 0;
}