#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

const int BUFF_SIZE = 512;

int main(int argc, char **argv){
    char *fifo_pathname = argv[1];
    char r_buff[BUFF_SIZE];

    mkfifo(fifo_pathname, 0666);

    int fifo_fd;
    
    if( (fifo_fd = open(fifo_pathname, O_RDONLY)) < 0){
        perror("failed to open fifo");
        exit(1);
    }

    while(read(fifo_fd, r_buff, BUFF_SIZE) > 0){
        printf("%s", r_buff);
    }
    

    

    if( (close(fifo_fd)) < 0){
        perror("failed to close fifo");
        exit(1);
    }

    exit(0);
}