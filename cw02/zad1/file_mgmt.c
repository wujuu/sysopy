#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include "file_mgmt.h"


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

    int fd = open(file_path, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR), i = 0;

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

    int processed_index, processed_key, tmp_key, i;

    char processed_buff[bytes], tmp_buff[bytes];

    if(if_sys){
        int fd = open(file_path, O_RDWR);

        if(fd == -1){
            err_sys("Open file failed!");
        }

        for(processed_index = 1; processed_index < records; processed_index++){

            lseek(fd, processed_index * bytes, SEEK_SET);

            if(read(fd, processed_buff, bytes) == -1){
                err_sys("Read from file failed!");
            }

            processed_key = processed_buff[0];

            lseek(fd, 0, SEEK_SET);
            int start_index = -1;

            do{
                if(read(fd, tmp_buff, bytes) == -1){
                    err_sys("Read from file failed!");
                }

                tmp_key = tmp_buff[0];
                start_index++;
            }
            while(tmp_key <= processed_key && start_index < processed_index);

            for(i = processed_index - 1; i >= start_index; i--){
                lseek(fd, i * bytes, SEEK_SET);

                if(read(fd, tmp_buff, bytes) == -1){
                    err_sys("Read from file failed!");
                }

                if(write(fd, tmp_buff, bytes) == -1){
                    err_sys("Write to file failed!");
                }

            }

            lseek(fd, start_index * bytes, SEEK_SET);
        
            if(write(fd, processed_buff, bytes) == -1){
                err_sys("Write to file failed!");
            }
        }

        close(fd);
    }
    else{
        FILE *file = fopen(file_path, "r+");

        if(file == NULL){
            err_sys("Open file failed!");
        }

        for(processed_index = 1; processed_index < records; processed_index++){

            fseek(file, processed_index * bytes, 0);

            if(fread(processed_buff, sizeof(char), bytes, file) == -1){
                err_sys("Read from file failed!");
            }

            processed_key = processed_buff[0];

            fseek(file, 0, 0);
            int start_index = -1;

            do{
                if(fread(tmp_buff, sizeof(char), bytes, file) == -1){
                    err_sys("Read from file failed!");
                }
                tmp_key = tmp_buff[0];
                start_index++;
            }
            while(tmp_key <= processed_key && start_index < processed_index);

            for(i = processed_index - 1; i >= start_index; i--){
                fseek(file, i * bytes, 0);

                if(fread(tmp_buff, sizeof(char), bytes, file) == -1){
                    err_sys("Read from file failed!");
                }

                if(fwrite(tmp_buff, sizeof(char), bytes, file) == -1){
                    err_sys("Write to file failed!");
                }

            }

            fseek(file, start_index * bytes, 0);
        
            if(fwrite(processed_buff, sizeof(char), bytes, file) == -1){
                err_sys("Write to file failed!");
            }
        }

        fclose(file);
    }
}


void copy(const char *orig_file_name, const char* new_file_name, int records, int bytes, int if_sys){
    char orig_file_path[256];
    strcpy(orig_file_path, "./");
    strcat(orig_file_path, orig_file_name);

    char new_file_path[256];
    strcpy(new_file_path, "./");
    strcat(new_file_path, new_file_name);

    int i;

    char buff[bytes];

    if(if_sys){
        int orig_fd = open(orig_file_path, O_RDONLY); 
        int new_fd = open(new_file_path, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR); 


        if(orig_fd == -1){
            err_sys("Open original file failed!");
        }

        if(new_fd == -1){
            err_sys("Create new file failed!");
        }

        for(i = 0; i < records; i++){
            if(read(orig_fd, buff, bytes) == -1){
                err_sys("Read from file failed!");
            }

            if(write(new_fd, buff, bytes) == -1){
                err_sys("Write to file failed!");
            }
        }



        close(orig_fd);
        close(new_fd);
    }
    else{
        FILE *orig_file = fopen(orig_file_path, "r");

        if(orig_file == NULL){
            err_sys("Open orignal file failed!");
        }

        FILE *new_file = fopen(new_file_path, "w");

        if(new_file == NULL){
            err_sys("Open new file failed!");
        }

        for(i = 0; i < records; i++){
            if(fread(buff, sizeof(char), bytes, orig_file) == -1){
                err_sys("Read from file failed!");
            }
            if(fwrite(buff, sizeof(char), bytes, new_file) == -1){
                err_sys("Write from file failed!");
            }
                    
        }

        fclose(orig_file);
        fclose(new_file);

    }
}
