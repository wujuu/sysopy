#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

pid_t global_child_pid;


void TSTP_parent_handler(int signo){
    printf("\nOczekuję na Ctrl+Z - kontynuacja albo CTR+C - zakonczenie programu\n");
    kill(global_child_pid, SIGKILL);
}


void TSTP_parent_handler2(int signo){
}

void INT_handler(int signo){
    printf("\nOdebrano sygnał SIGINT\n");
    kill(global_child_pid, SIGKILL);
    exit(1);
}



int main(){



    while(1){
        signal(SIGINT, SIG_IGN);

        pid_t child_pid;

        if((child_pid = fork()) < 0){
            perror("Failed to fork!");
            exit(1);
        }
        else if(child_pid == 0){

            sigset_t blocked_signals;

            sigemptyset(&blocked_signals);

            sigaddset(&blocked_signals, SIGTSTP);

            sigaddset(&blocked_signals, SIGINT);

            sigprocmask(SIG_SETMASK, &blocked_signals, NULL);

            if(execl("/home/patryk/Documents/IET/Sysopy/sysopy/cw04/zad2/endless_date.sh", "", NULL) < 0){
                perror("Couldn't start this program");
                exit(1);
            }

            exit(0);
        }
        else if (child_pid > 0){

            global_child_pid = child_pid;
            

            
            signal(SIGTSTP, &TSTP_parent_handler);
            wait(NULL);
            signal(SIGINT, INT_handler);
            signal(SIGTSTP, &TSTP_parent_handler2);
            pause();
            
        }
    }
    
    exit(0);
}