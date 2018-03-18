#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include "file_mgmt.h"


int main(int argc, const char **argv){
    srand(time(NULL));

    int records, bytes;
    const char* sys = "sys";
    const char *lib = "lib";

    if(strcmp(argv[1], "generate") == 0){

        if(argc != 5){
            err_sys("Wrong number of arguments!");
        }

        if((records = atoi(argv[3])) <= 0){
            err_sys("Wrong records size!");
        }

        if((bytes = atoi(argv[4])) <= 0){
            err_sys("Wrong bytes size!");
        }

        generate(argv[2], records, bytes);
    }
    else if (strcmp(argv[1], "sort") == 0){

        if(argc != 6){
            err_sys("Wrong number of arguments!");
        }

        if((records = atoi(argv[3])) <= 0){
            err_sys("Wrong records size!");
        }

        if((bytes = atoi(argv[4])) <= 0){
            err_sys("Wrong bytes size!");
        }

        if(strcmp(argv[5], lib) == 0){
            sort(argv[2], records, bytes, 0);
        }
        else if (strcmp(argv[5], sys) == 0){
            sort(argv[2], records, bytes, 1);
        }
        else{
            err_sys("Wrong implementation option!");
        }

    }
    else if (strcmp(argv[1], "copy") == 0){

        if(argc != 7){
            err_sys("Wrong number of arguments!");
        }

        if( access( argv[2], F_OK ) == -1 ) {
            err_sys("This file doesn't exist!");
        }

        if((records = atoi(argv[4])) <= 0){
            err_sys("Wrong records size!");
        }

        if((bytes = atoi(argv[5])) <= 0){
            err_sys("Wrong bytes size!");
        }

        if(strcmp(argv[6], lib) == 0){
            copy(argv[2], argv[3], records, bytes, 0);
        }
        else if (strcmp(argv[6], sys) == 0){
            copy(argv[2], argv[3], records, bytes, 1);
        }
        else{
            err_sys("Wrong implementation option!");
        }

    }
    else{
        err_sys("Wrong action!");
    }


    return 0;
}

