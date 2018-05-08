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
    printf("%i ", getpid());
    print_time_msg("Leaving barbershop...\n");
}

int check_state(){
    int state = 0;

    wait_semaphore(barbershop_sem);

    barbershop_pointer = shmat(shm_id, NULL, 0);

    state = barbershop_pointer -> state;

    shmdt(barbershop_pointer);

    post_semaphore(barbershop_sem);

    return state;
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

void insert_client(struct client new_client){

    barbershop_pointer -> passed_client = new_client;

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

        if(child_pid == 0){
            //SINGLE CLIENT BEHAVIOUR
            struct client new_client = init_client(getpid(), times_to_cut);

            while(new_client.times_cut != new_client.times_to_cut){
                wait_semaphore(barbershop_sem);
                barbershop_pointer = shmat(shm_id, NULL, 0);

                int state = barbershop_pointer -> state;

                int left = 0;

                if(state == 0){
                    barbershop_pointer -> state = 1;
                    barbershop_pointer -> passed_client = new_client;
                    printf("%i ", getpid());
                    print_time_msg("Sitting at the chair...\n");
                    kill(barber_pid, SIGUSR1);

                } else {
                    int full = is_full();

                    if(!full){
                        printf("%i ", getpid());
                        print_time_msg("Going to the queue...\n");
                        push_clients_queue(&(barbershop_pointer -> queue), new_client);
                    } else {
                        printf("%i ", getpid());
                        print_time_msg("Leaving barbershop - no places in the queue...\n");
                        left = 1;
                    }
                }
                shmdt(barbershop_pointer);
                post_semaphore(barbershop_sem);

                if(!left){
                    sigsuspend(&null_block);
                    new_client.times_cut++;
                }
                printf("%i ", getpid());
                printf("Times shaven: %i\n", new_client.times_cut);
            }
            
            exit(0);
        }
    }
    
    pid_t tmp_pid;
    while (  (tmp_pid = waitpid(-1, NULL, 0) )) {
        if (errno == ECHILD) {
            break;
   }
}

    exit(0);
}