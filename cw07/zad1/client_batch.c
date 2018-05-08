#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>
#include <errno.h>

#include "single_sem.h"
#include "clients_queue.h"
#include "barbershop.h"

int shm_id = -1;
struct semaphore *barbershop_sem = NULL;
struct barbershop *barbershop_pointer = NULL;

sigset_t sigusr2_block, null_block;



void sigusr2_handler(int sig_no){
    
}



pid_t get_barber_pid(){
    pid_t barber_pid = -1;

    wait_semaphore(barbershop_sem);

    barbershop_pointer = shmat(shm_id, NULL, 0);

    barber_pid = barbershop_pointer -> barber_pid;

    shmdt(barbershop_pointer);

    post_semaphore(barbershop_sem);

    return barber_pid;
}


int is_full(){
    int full = 0;


    barbershop_pointer = shmat(shm_id, NULL, 0);

    if(barbershop_pointer -> queue.clients_amount == barbershop_pointer -> max_clients) 
        full = 1;


    return full;
}

int main(int argc, char **argv){
    //SIGNALS
    signal(SIGUSR2, sigusr2_handler);

    sigemptyset(&sigusr2_block);
    sigaddset(&sigusr2_block, SIGUSR2);
    sigprocmask(SIG_SETMASK, &sigusr2_block, NULL);

    sigemptyset(&null_block);


    //PARSING SOME USEFUL ARGUMENTS
    char *home = getenv("HOME");
    int clients = atoi(argv[1]);
    int times_to_cut = atoi(argv[2]);

    //OPENING SHARED MEMORY SEGMENT
    key_t shm_key = ftok(home, 1);
    shm_id = shmget(shm_key, 0, 0);

    //OPENING SEMAPHORE
    barbershop_sem = open_semaphore(home);

    //SHARED MEMORY AND SEMAPHORE WORKS FROM HERE
    
    pid_t barber_pid = get_barber_pid();

    //MAIN CLIENT BATCH LOOP
    pid_t child_pid;

    for(int i = 0; i < clients; i++){
        child_pid = fork();

        //SINGLE CLIENT BEHAVIOUR
        if(child_pid == 0){

            //INITALIZING NEW CLIENT STRUCTURE
            struct client new_client = init_client(getpid(), times_to_cut);

            //MAIN CLIENT LOOP
            while(new_client.times_cut != new_client.times_to_cut){
                wait_semaphore(barbershop_sem); //WAITING SEMAPHORE FOR ONE CLIENT LOOP ITERATION
                barbershop_pointer = shmat(shm_id, NULL, 0);

                //CHECKING IF BARBER IS SHAVING OR SLEEPING
                int state = barbershop_pointer -> state;

                //FLAG IF CLIENT LEFT THE BARBERSHOP WHEN THERE ARE NO PLACES IN THE QUEUE
                int left = 0;

                
                if(state == 0){ //IMMEDIATE SHAVING

                    barbershop_pointer -> state = 1; //CHANGING THE STATE OF THE BARBER TO SHAVING
                    barbershop_pointer -> passed_client = new_client; //FOR KNOWING THE IMMEDIATE CLIENT PID

                    printf("%i ", getpid());
                    print_time_msg("Waking up barber...\n");

                    kill(barber_pid, SIGUSR1); //SENDING WAKE UP SIGNAL TO THE BARBER

                } else { //QUEUE ENTERING
                    int full = is_full();

                    if(!full){
                        printf("%i ", getpid());
                        print_time_msg("Going to the queue...\n");

                        //PUSHING THE CLIENT TO THE QUEUE
                        push_clients_queue(&(barbershop_pointer -> queue), new_client);

                    } else {
                        printf("%i ", getpid());
                        print_time_msg("Leaving barbershop, no places in the queue...\n");


                        left = 1;
                    }
                }
                shmdt(barbershop_pointer);
                post_semaphore(barbershop_sem);
                
                //WAITING FOR BARBER DONE SHAVING SIGNAL
                if(!left){                
                    sigsuspend(&null_block);
                    new_client.times_cut++;

                    printf("%i ", getpid());
                    print_time_msg("Leaving barbershop, done shaving no ");
                    printf("%i...\n", new_client.times_cut);
                    
                }
            }
            
            exit(0);
        }
    }
    
    //WAITNG FOR ALL CHILDREN TO FINISH
    pid_t tmp_pid;
    while (  (tmp_pid = waitpid(-1, NULL, 0) )) {
        if (errno == ECHILD) {
            break;
   }
}

    exit(0);
}