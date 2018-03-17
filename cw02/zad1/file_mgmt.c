#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

const int BUFF_SIZE = 12;

void err_sys(const char *x) 
{ 
    perror(x); 
    exit(1); 
}


void fill_random(char *to_fill, int size){
    int i = 0;

    for(i = 0; i < size - 1; i++){
        to_fill[i] = 97 + rand() % 26;
    }

    to_fill[size - 1] = 10;
}


char *get_random(int size){
    char *rand_str = malloc(size * sizeof(char));
    
    fill_random(rand_str, size);

    return rand_str;
}


void generate(const char *file_name, int records, int bytes){
    char file_path[256];
    strcpy(file_path, "./");
    strcat(file_path, file_name);

    int fd = open(file_path, O_WRONLY | O_CREAT | O_EXCL | O_APPEND, S_IRUSR | S_IWUSR), i = 0;

    if(fd == -1){
        err_sys("Create file failed!");
    }

    for(i = 0; i < records; i++){
        char *w_buff = get_random(bytes);

        if((write(fd, w_buff, bytes)) == -1){
            err_sys("Write to file failed!");
        }
    }

    close(fd);
}


void sort(const char *file_name, int records, int bytes, int if_sys){
    char file_path[256];
    strcpy(file_path, "./");
    strcat(file_path, file_name);



    int fd = open(file_path, O_RDWR), processed_index, processed_key, tmp_key, i;
    char processed_buff[bytes], tmp_buff[bytes];

    if(fd == -1){
        err_sys("Open file failed!");
    }


    for(processed_index = 1; processed_index < records; processed_index++){

        lseek(fd, processed_index * bytes, SEEK_SET);

        read(fd, processed_buff, bytes);

        processed_key = processed_buff[0];

        lseek(fd, 0, SEEK_SET);
        int start_index = -1;

        do{
            read(fd, tmp_buff, bytes);
            tmp_key = tmp_buff[0];
            start_index++;
        }
        while(tmp_key <= processed_key && start_index < processed_index);

        for(i = processed_index - 1; i >= start_index; i--){
            lseek(fd, i * bytes, SEEK_SET);
            read(fd, tmp_buff, bytes);
            write(fd, tmp_buff, bytes);

        }

        lseek(fd, start_index * bytes, SEEK_SET);
    
        write(fd, processed_buff, bytes);
    }


    close(fd);
}


void copy(const char *orig_file_name, const char* new_file_name, int records, int bytes, int if_sys){
    char orig_file_path[256];
    strcpy(orig_file_path, "./");
    strcat(orig_file_path, orig_file_name);

    char new_file_path[256];
    strcpy(new_file_path, "./");
    strcat(new_file_path, new_file_name);

    int orig_fd = open(orig_file_path, O_RDONLY), new_fd = open(new_file_path, O_WRONLY | O_CREAT | O_EXCL | O_APPEND, S_IRUSR | S_IWUSR), i;

    char buff[bytes];

    if(orig_fd == -1){
        err_sys("Can't open original file!");
    }

    if(new_fd == -1){
        err_sys("Create new file failed!");
    }

    for(i = 0; i < records; i++){
        if(read(orig_fd, buff, bytes) == -1){
            err_sys("Read file failed!");
        }

        if(write(new_fd, buff, bytes) == -1){
            err_sys("Write file failed!");
        }
    }



    close(orig_fd);
    close(new_fd);
}



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