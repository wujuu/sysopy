#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include "single_sem.h"


int main(){
    char *home = getenv("HOME");

    struct semaphore *sem = open_semaphore(home);

    key_t shm_key = ftok(home, 1);
    int shm_id = shmget(shm_key, 0, 0);
    int *p = NULL;
    int n = -1;

    print_sem_stats(sem);

    do{
        wait_semaphore(sem);
        p = shmat(shm_id, NULL, 0);
        n = *p;

        printf("%i\n", n);

        post_semaphore(sem);
    } while(n != 3);

    

    exit(0);
}
