#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>



int main(){
    int BUFF_SIZE = 512;

    char buff[BUFF_SIZE];
    int fd[2], fd_middle[2];
    pid_t child_pid1, child_pid2, child_pid3;

    if(pipe(fd) < 0){
        perror("Pipe error!");
        exit(1);
    } 

    if(pipe(fd_middle) < 0){
        perror("Pipe error!");
        exit(1);
    } 

    if((child_pid1 = fork()) < 0 ){
        perror("Fork error!");
        exit(1);
    } else if (child_pid1 == 0){
        close(fd[1]);
        dup2(fd[0], STDIN_FILENO);


        close(fd_middle[0]);
        dup2(fd_middle[1], STDOUT_FILENO);


        execlp("grep", "grep", "Q", NULL);

        exit(0);
    }


    close(fd[0]);
    close(fd_middle[1]);
    
    // write(fd[1], "QWE\nASD", 3);
    //read(fd_middle[0], buff, BUFF_SIZE);

    wait(NULL);

    // printf("%s", buff);

}