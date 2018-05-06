#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include "single_sem.h"

void remove_semaphore(struct semaphore *sem){
    if((semctl(sem->id, 0, IPC_RMID)) < 0){
        perror("Couldn't remove the semaphore!");
    }
}

struct semaphore *init_semaphore(char *name){

    key_t key;
    int id;

    if ((key = ftok(name, 1)) < 0) {
        perror("Couldn't get the key for the semaphore");
        exit(1);
    }

    if((id = semget(key, 1, IPC_CREAT | PERMS)) < 0){
        perror("Couldn't create the semaphore");
        exit(1);
    }


    struct semaphore *new_sem = malloc(sizeof(struct semaphore));
    new_sem -> id = id;


    struct sembuf *semoparray = malloc(sizeof(struct sembuf));

    semoparray[0].sem_op = 1;
    semoparray[0].sem_flg = 0;
    semoparray[0].sem_flg = 0;

    if((semop(id, semoparray, 1) ) < 0){
        perror("Couldn't initalize the semaphore");
        remove_semaphore(new_sem);
        exit(1);        
    }

    return new_sem;
}

struct semaphore *open_semaphore(char* name){
    key_t key;
    int id;

    if ((key = ftok(name, 1)) < 0) {
        perror("Couldn't get the key for the semaphore");
        exit(1);
    }

    if((id = semget(key, 1, 0)) < 0){
        perror("Couldn't create the semaphore");
        exit(1);
    }

    struct semaphore *new_sem = malloc(sizeof(struct semaphore));
    new_sem -> id = id;

    return new_sem;
}

void post_semaphore(struct semaphore *sem){
    struct sembuf *semoparray = malloc(sizeof(struct sembuf));

    //RETURNING RESOURCES
    semoparray[0].sem_op = 1;
    semoparray[0].sem_flg = 0;
    semoparray[0].sem_flg = 0;

    if( (semop(sem -> id, semoparray, 1) ) < 0){
        perror("Couldn't post to the semaphore");
    }
}

void wait_semaphore(struct semaphore *sem){
    struct sembuf *semoparray = malloc(sizeof(struct sembuf));

    //REQUESTING AND POSSIBLY WAITING FOR RESOURCES
    semoparray[0].sem_op = -1;
    semoparray[0].sem_flg = 0;
    semoparray[0].sem_flg = 0;

    if( (semop(sem -> id, semoparray, 1) ) < 0){
        perror("Couldn't wait for the semaphore");
    }
}

void print_sem_stats(struct semaphore *sem){
    union semun sem_info;

    int sem_val = 0;

    if((sem_val = semctl(sem -> id, 0, GETVAL, sem_info)) < 0){
        perror("Failed to get semaphore stats!");
        exit(1);
    }

    pid_t last_pid;

    if((last_pid = semctl(sem -> id, 0, GETPID, sem_info)) < 0){
        perror("Failed to get semaphore stats!");
        exit(1);
    }

    printf("- Value: %i\n", sem_val);
    printf("- Last operator PID: %i\n", last_pid);
    printf("\n");
}