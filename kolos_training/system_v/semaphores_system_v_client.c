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

    int sem_id = semget(sem_key, 0, 0000);


    struct sembuf *sops = malloc(sizeof(struct sembuf));

    sops -> sem_num = 0;
    sops -> sem_op = -1;
    sops -> sem_flg = 0;

    semop(sem_id, sops, 1);

    char txt[10];
    scanf("%s", txt);

    sops -> sem_num = 0;
    sops -> sem_op = 1;
    sops -> sem_flg = 0;

    semop(sem_id, sops, 1);

    exit(0);
}