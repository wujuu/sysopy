#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dblocks.h"
#include "sblocks.h"






int main(int argc, char** argv){
    //10 10 static add delete find 20 10 1999

    int blocksSize = atoi(argv[1]);
    int charsSize = atoi(argv[2]);
    char* alloc_type = argv[3]; //static / dynamic
    char** actions = calloc(3, sizeof(char));
    int* indexs = calloc(3, sizeof(int));

    const char* dynamic = "dynamic";

    for(int i = 0; i<3; i++){
        actions[i] = argv[i + 4];
    }

     for(int i = 0; i<3; i++){
         indexs[i] = atoi(argv[i + 7])
     }



    if(strcmp(alloc_type, dynamic) == 0){
        charBlockArray* dynamicArray = makeRandCharBlockArray(blocksSize, charsSize);
        
        //printCharBlockArray(dynamicArray);

        for(int i = 0; i<3; i++){
            if(strcmp(alloc_type, dynamic) == 0)
        }
    }
    else{
        staticCharBlockArray staticArray = staticMakeRandCharBlockArray(blocksSize, charsSize);

        staticPrintCharBlockArray(staticArray);
    }

    return 0;
}