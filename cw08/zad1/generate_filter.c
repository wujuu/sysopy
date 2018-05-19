#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>



int main(int argc, char **argv){

    FILE *fp = fopen(argv[2], "w");

    int size = atoi(argv[1]);

    char buff[256];
    char tmp[256];
    for(int i = 0; i < 256; i++) {
        buff[i] = '\0';
        tmp[i] = '\0';

    }

    for(int i = 0; i < size/2; i++){
        sprintf(tmp, "%d ", -1);
        strcat(buff, tmp);
    }

    for(int i = size/2; i < size; i++){
        sprintf(tmp, "%d ", 1);
        strcat(buff, tmp);
    }

    fprintf(fp, "%i\n", size);

    for(int i = 0; i < size; i++)
        fprintf(fp, "%s\n" ,buff);


    exit(0);
}