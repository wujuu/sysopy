#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

pid_t global_parent_pid, global_child_pid;
int signals_child_recieved;
int signals_parent_recieved;
int signals_parent_sent;

void usr1_child_handler(int signo){
    signals_child_recieved++;
    kill(global_parent_pid, SIGUSR1);
    printf("Hey, father!\n");
}

void usr1_parent_handler(int signo){
    printf("Hey, child!\n");
    signals_parent_recieved++;
}

void usr2_child_handler(int signo){
    printf("Goodbye!\n");
    printf("Signals child recieved: %i\n", signals_child_recieved);

    
    exit(0);
}

void int_parent_handler(int signo){
    kill(global_child_pid, SIGUSR2);
}

int main(int argc, char ** argv){

    int L = atoi(argv[1]);
    int type = atoi(argv[2]);

    global_parent_pid = getpid();

    pid_t child_pid;


    sigset_t no_usr2_block;
    sigfillset(&no_usr2_block);
    sigdelset(&no_usr2_block, SIGUSR2);
    sigprocmask(SIG_SETMASK, &no_usr2_block, NULL);

    sigset_t no_usr_block;
    sigfillset(&no_usr_block);
    sigdelset(&no_usr_block, SIGUSR1);
    sigdelset(&no_usr_block, SIGUSR2);

    sigset_t empty_mask;
    sigemptyset(&empty_mask);



    if((child_pid = fork()) < 0){
        perror("Fork failed!");
        exit(1);
    }



    else if (child_pid == 0){

        signal(SIGUSR1, &usr1_child_handler);
        signal(SIGUSR2, &usr2_child_handler);
        
        if(type == 1){
            sigprocmask(SIG_SETMASK, &no_usr_block, NULL);
        }
        else if(type == 2){
            for(int i = 0; i < L; i++){
                sigsuspend(&no_usr_block);
            }
        }
        
        pause();

        exit(0);
    }
    else if (child_pid > 0){
        sigprocmask(SIG_SETMASK, &empty_mask, NULL);
        global_child_pid = child_pid;
        signal(SIGUSR1, &usr1_parent_handler);
        signal(SIGINT, &int_parent_handler);


        for(int i = 0; i < L; i++){
            kill(child_pid, SIGUSR1);
            signals_parent_sent++;

            if(type == 2){
                pause();
            }
        }

        kill(child_pid, SIGUSR2);

        wait(NULL);

    }

    printf("Signals parent sent: %i\n", signals_parent_sent);
    printf("Signals parent recieved: %i\n", signals_parent_recieved);


    exit(0);
}
