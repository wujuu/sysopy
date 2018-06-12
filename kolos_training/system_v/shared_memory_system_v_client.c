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

    int shm_id = shmget(shm_key, 0, 0);

    struct shared_struct *got_shared_struct = shmat(shm_id, NULL, 0);

    got_shared_struct -> x = 500;
    got_shared_struct -> y = 600;
    got_shared_struct -> wtf = 'a';

    shmdt(got_shared_struct);

    exit(0);
}