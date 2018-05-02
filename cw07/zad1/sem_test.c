#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "single_sem.h"


int main(){
    char str[10];
    char *home = getenv("HOME");

    //SEM INIT
    struct semaphore *sem = init_semaphore(home);

    //SHM INIT
    key_t shm_key = ftok(home, 1);
    int shm_id = shmget(shm_key, sizeof(int), IPC_CREAT | PERMS);
    int *p = NULL;


    wait_semaphore(sem);

    scanf("%s", str);

    post_semaphore(sem);

    p = shmat(shm_id, NULL, 0);

    for(int i = 1; i < 4; i++){
        wait_semaphore(sem);
        *p = i;
        post_semaphore(sem);
    }

    scanf("%s", str);

    if ((shmctl(shm_id, IPC_RMID, NULL)) < 0){
        perror("WTF");
    }
    remove_semaphore(sem);


    exit(0);
}
