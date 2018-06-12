#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

struct shared_struct {
    int x, y;
    char wtf;
};

int main(){
    const char pathname[] = "home/partyk/Downloads";
    int proj = 100;

    key_t shm_key = ftok(pathname, proj);

    int shm_id = shmget(shm_key, sizeof(struct shared_struct), IPC_CREAT | S_IRWXU);

    struct shared_struct *got_shared_struct = shmat(shm_id, NULL, 0);

    got_shared_struct -> x = 10;
    got_shared_struct -> y = 20;
    got_shared_struct -> wtf = 'w';

    shmdt(got_shared_struct);

    char txt[10];
    scanf("%s", txt);

    got_shared_struct = shmat(shm_id, NULL, 0);

    printf("%i\n", got_shared_struct -> x);
    printf("%i\n", got_shared_struct -> y);
    printf("%c\n", got_shared_struct -> wtf);

    shmdt(got_shared_struct);

    shmctl(shm_id, IPC_RMID, NULL);

    exit(0);
}