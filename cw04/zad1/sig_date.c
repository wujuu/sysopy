#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>

void SIGTSTP_handler1(int sig);


void SIGTSTP_handler2(int sig){
    struct sigaction new_action;

    sigset_t block_mask;
    sigemptyset(&block_mask);

    new_action.sa_mask = block_mask;
    new_action.sa_flags = SA_NODEFER;
    new_action.sa_handler = SIGTSTP_handler1;

    sigaction(SIGTSTP, &new_action, NULL);
}

void SIGTSTP_handler1(int sig){
    printf("Oczekuję na Ctrl+Z - kontynuacja albo CTR+C - zakonczenie programu\n");
    signal(SIGTSTP, SIGTSTP_handler2);
    pause();
}

void sigint_handler(int sig){
    prtinf("Odebrano sygnał SIGINT\n");
    exit(0);
}


int main(){

    struct sigaction new_action;

    sigset_t block_mask;
    sigemptyset(&block_mask);

    new_action.sa_mask = block_mask;
    new_action.sa_flags = SA_NODEFER;
    new_action.sa_handler = SIGTSTP_handler1;



    sigaction(SIGTSTP, &new_action, NULL);
    signal(SIGINT, sigint_handler);

    time_t raw_now;
    struct tm *nice_now;

    while(1){
        
        time(&raw_now);
        nice_now = localtime(&raw_now);

        printf("%s", asctime(nice_now));

        sleep(1);
    }
    
}