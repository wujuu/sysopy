#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <string.h>

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

int main(){
    const char pathname[] = "home/partyk/Documents";
    int proj = 100;

    

    key_t sem_key = ftok(pathname, proj);

    int sem_id = semget(sem_key, 1, IPC_CREAT | S_IRWXU);

    union semun sem_info;

    sem_info.val = 1;

    semctl(sem_id, 0, SETVAL, sem_info);

    char txt[10];
    scanf("%s", txt);

    struct sembuf *sops = malloc(sizeof(struct sembuf));

    sops -> sem_num = 0;
    sops -> sem_op = -1;
    sops -> sem_flg = 0;

    semop(sem_id, sops, 1);

    semctl(sem_id, 0, IPC_RMID);

    exit(0);


}