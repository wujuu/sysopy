#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>

#include "single_sem.h"
#include "clients_queue.h"
#include "barbershop.h"

int main(){
    //PARSING SOME USEFUL ARGUMENTS
    char *home = getenv("HOME");

    //OPENING SHARED MEMORY SEGMENT
    key_t shm_key = ftok(home, 1);
    int shm_id = shmget(shm_key, 0, 0);

    //OPENING SEMAPHORE
    struct semaphore *barbershop_sem = open_semaphore(home);

    struct barbershop *new_barbershop = NULL;

    wait_semaphore(barbershop_sem);

    new_barbershop = shmat(shm_id, NULL, 0);
    shmdt(new_barbershop);

    post_semaphore(barbershop_sem);

    return 0;
}