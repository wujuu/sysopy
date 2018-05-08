#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>
#include <signal.h>

#include "single_sem.h"
#include "clients_queue.h"
#include "barbershop.h"

//CONST VARIABLES FOR SIGNAL HANDLER
int shm_id = -1;
struct semaphore *barbershop_sem = NULL;
struct barbershop *barbershop_pointer = NULL;

sigset_t sigusr1_block, null_block;


struct barbershop init_barbershop(int max_clients){
    struct barbershop new_barbershop;
    new_barbershop.state = 0;
    new_barbershop.max_clients = max_clients;
    new_barbershop.queue = init_clients_queue();
    new_barbershop.barber_pid = getpid();

    return new_barbershop;
}

void sigint_handler(int sig_no){

    printf("\n");

    print_time_msg("Terminating barber process...\n");

    //REMOVING SHARED MEMORY SEGMENT
    shmctl(shm_id, IPC_RMID, NULL);

    //REMMOVING SEMAPHORE
    remove_semaphore(barbershop_sem);

    exit(0);
}

void sigusr1_handler(int sig_no){
    print_time_msg("Barber is being woken up...\n");
}


void fall_asleep(){
    print_time_msg("Barber falling asleep...\n");
}

void cut_moustache(struct client *shaven_client){
    pid_t shaven_pid = shaven_client -> pid;
    print_time_msg("Barber shaving client ");
    printf("%i...\n", shaven_pid);
    print_time_msg("Done shaving client ");
    printf("%i...\n", shaven_pid);
}



//NO ERROR WARNINGS ETC FOR NOW
int main(int argc, char **argv){
    //PREPARING HANDLERS
    signal(SIGINT, sigint_handler);
    signal(SIGUSR1, sigusr1_handler);

    //BLOCKING SIGNALS (FOR SIGSUSPEND)
    sigemptyset(&sigusr1_block);
    sigaddset(&sigusr1_block, SIGUSR1);
    sigprocmask(SIG_SETMASK, &sigusr1_block, NULL);

    sigemptyset(&null_block);

    //PARSING SOME USEFUL ARGUMENTS
    int max_clients = atoi(argv[1]);
    char *home = getenv("HOME");

    //INITIALIZING SHARED MEMORY SEGMENT
    key_t shm_key = ftok(home, 1);
    shm_id = shmget(shm_key, sizeof(struct barbershop), IPC_CREAT | PERMS);

    //INITALIZING SEMAPHORE
    barbershop_sem = init_semaphore(home);

    struct client current_client;
    //INITIALIZING SHARED BARBERSHOP STRUCTURE
    wait_semaphore(barbershop_sem);

    barbershop_pointer = shmat(shm_id, NULL, 0);
    *barbershop_pointer = init_barbershop(max_clients);

    //shmdt(barbershop_pointer);

    
    //MAIN BARBER LOOP GOES HERE
    while(1){
        fall_asleep();

        shmdt(barbershop_pointer);
        post_semaphore(barbershop_sem);

        //WAITING FOR WAKEUP SIGNAL
        sigsuspend(&null_block);

        //getting client
        wait_semaphore(barbershop_sem);
        barbershop_pointer = shmat(shm_id, NULL, 0);
        current_client = barbershop_pointer -> passed_client;
        shmdt(barbershop_sem);
        post_semaphore(barbershop_sem);


        cut_moustache(&current_client);

        wait_semaphore(barbershop_sem);

        kill(current_client.pid, SIGUSR2);

        barbershop_pointer = shmat(shm_id, NULL, 0);

        while(1){
            if(!is_empty(barbershop_pointer -> queue)){
                current_client = pop_clients_queue(&(barbershop_pointer -> queue));
                shmdt(barbershop_pointer);
                post_semaphore(barbershop_sem);

                cut_moustache(&current_client);

                wait_semaphore(barbershop_sem);

                kill(current_client.pid, SIGUSR2);

                barbershop_pointer = shmat(shm_id, NULL, 0);

            } else {
                barbershop_pointer -> state = 0;
                break;
            }
        }

    }

    //EXIT ONLY THROUGH SIGINT
}