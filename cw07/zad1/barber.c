#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>

#include "single_sem.h"
#include "clients_queue.h"
#include "barbershop.h"




struct barbershop init_barbershop(int max_clients){
    struct barbershop new_barbershop;
    new_barbershop.state = 0;
    new_barbershop.max_clients = max_clients;
    new_barbershop.queue = init_clients_queue();

    return new_barbershop;
}



//NO ERROR WARNINGS ETC FOR NOW
int main(int argc, char **argv){

    //PARSING SOME USEFUL ARGUMENTS
    int max_clients = atoi(argv[1]);
    char *home = getenv("HOME");

    //INITIALIZING SHARED MEMORY SEGMENT
    key_t shm_key = ftok(home, 1);
    int shm_id = shmget(shm_key, sizeof(struct barbershop), IPC_CREAT | PERMS);

    //INITALIZING SEMAPHORE
    struct semaphore *barbershop_sem = init_semaphore(home);

    
    //INITIALIZING SHARED BARBERSHOP STRUCTURE
    struct barbershop *barbershop_pointer = NULL;

    wait_semaphore(barbershop_sem);

    barbershop_pointer = shmat(shm_id, NULL, 0);
    *barbershop_pointer = init_barbershop(max_clients);

    shmdt(barbershop_pointer);

    post_semaphore(barbershop_sem);


    //MAIN BARBER LOOP GOES HERE






    //REMOVING SHARED MEMORY SEGMENT
    shmctl(shm_id, IPC_RMID, NULL);

    //REMMOVING SEMAPHORE
    remove_semaphore(barbershop_sem);

    exit(0);
}