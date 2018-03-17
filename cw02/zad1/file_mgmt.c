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
        to_fill[i] = 32 + rand() % 95;
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
        err_sys("Open file failed!");
    }

    for(i = 0; i < records; i++){
        char* w_buf = get_random(bytes);

        if((write(fd, w_buf, bytes)) == -1){
            err_sys("Write to file failed!");
        }
    }

    close(fd);
}


int main(){
    srand(time(NULL));

    // char r_buf[BUFF_SIZE];
    // char w_buf[BUFF_SIZE];

    // fill_random(w_buf, BUFF_SIZE);

    // int fd = open("./test_file.txt", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR), i = 0, b_read, b_writt;


    // // if((b_read = read(fd, r_buf, BUFF_SIZE)) == -1){
    // //     err_sys("Read from file failed!");
    // // }


    // if((b_writt = write(fd, w_buf, BUFF_SIZE)) == -1){
    //     err_sys("Write to file failed!");
    // }


    // close(fd);

    generate("name2", 100, 512);

    return 0;
}