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
    const char* add = "add"; 
    const char* del = "del";
    const char* find = "find";

    for(int i = 0; i<3; i++){
        actions[i] = argv[i + 4];
    }

     for(int i = 0; i<3; i++){
         indexs[i] = atoi(argv[i + 7]);
     }
     

    if(strcmp(alloc_type, dynamic) == 0){
        charBlockArray* dynamicArray = makeRandCharBlockArray(blocksSize, charsSize);

        for(int i = 0; i<3; i++){
            if(strcmp(actions[i], add) == 0){
                char* tmp = calloc(charsSize, sizeof(char));

                for(int j = 0; j < charsSize; j++){
                    tmp[j] = 32 + rand() % 95;
                }

                addBlock(dynamicArray, indexs[i], charsSize, tmp);

                printf("added random blocks at %i index\n", indexs[i]);
            }

            if(strcmp(actions[i], del) == 0){
                delBlock(dynamicArray, indexs[i]);
                printf("delete blocks at %i index\n", indexs[i]);

            }

            if(strcmp(actions[i], find) == 0){
                getMinBlock(dynamicArray, indexs[i]);
                printf("found closest sum of chars at blocks %i\n", indexs[i]);
            }
        }
    }
    else{
        staticCharBlockArray staticArray = staticMakeRandCharBlockArray(blocksSize, charsSize);

        for(int i = 0; i<3; i++){
            if(strcmp(actions[i], add) == 0){
                char* tmp = calloc(charsSize, sizeof(char));

                for(int j = 0; j < charsSize; j++){
                    tmp[j] = 32 + rand() % 95;
                }

                staticAddBlock(&staticArray, indexs[i], charsSize, tmp);

                 printf("added random blocks at %i index\n", indexs[i]);
            }

            if(strcmp(actions[i], del) == 0){
                staticDelBlock(&staticArray, indexs[i]);

                printf("delete blocks at %i index\n", indexs[i]);

            }

            if(strcmp(actions[i], find) == 0){
                staticGetMinIndex(staticArray, indexs[i]);

                printf("found closest sum of chars to %i at blocks\n", indexs[i]);
            }
        }



    }

    return 0;
}