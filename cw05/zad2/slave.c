#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

const int BUFF_SIZE = 512;

int main(int argc, char **argv){
    char *fifo_pathname = argv[1];
    int N = atoi(argv[2]);

    int fifo_fd;

    int slave_pid = getpid();

    char str_slave_pid[BUFF_SIZE];

    sprintf(str_slave_pid, "%d", slave_pid);
    
    if( (fifo_fd = open(fifo_pathname, O_WRONLY)) < 0){
        perror("failed to open fifo");
        exit(1);
    }

    for(int i = 0; i < N; i++){
        printf("Writing to fifo...\n");

        FILE *date_stream = popen("date", "r");
        char r_buff[BUFF_SIZE];
        int bytes_read = 0;
        if((bytes_read = fread(r_buff, BUFF_SIZE, 1, date_stream)) < 0){
            perror("fread failed!");
            exit(1);
        }
        pclose(date_stream);

        char message[2*BUFF_SIZE];
        strcpy(message, str_slave_pid);
        strcat(message, " ");
        strcat(message, r_buff);

        if(write(fifo_fd, message, 2*BUFF_SIZE) < 0){
            perror("failed to write to fifo");
            exit(1);
        }

        int seconds_to_sleep = 2 + (rand() % 4);
        sleep(seconds_to_sleep);
    }


    if((close(fifo_fd)) < 0){
        perror("failed to close fifo");
        exit(1);
    }

    exit(0);
}