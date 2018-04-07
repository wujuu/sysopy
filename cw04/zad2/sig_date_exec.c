#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>




int main(){

    pid_t child_pid;

    if((child_pid = fork()) < 0){
        perror("Failed to fork!");
        exit(1);
    }
    else if(child_pid == 0){
        printf("I'm a child!\n");

        if(execl("/home/patryk/Documents/IET/Sysopy/sysopy/cw04/zad2/endless_date.sh", "", NULL) < 0){
            perror("Couldn't start this program");
            exit(1);
        }

        exit(0);
    }
    else if (child_pid > 0){
        signal(SIGTSTP, SIG_IGN);
        wait(NULL);
        printf("My child finished execution!\n");
    }
    
    exit(0);
}