#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>



int main(){

    sem_t *my_semaphore;
    const char name[] = "/my_semaphore1";

    my_semaphore = sem_open(name, O_RDWR | O_CREAT, S_IRWXU, 1);

    if( my_semaphore == SEM_FAILED  ){
        perror("Couldn't open the semaphore");
        exit(1);
    }

    char txt[10];
    scanf("%s", txt);


    if(sem_wait(my_semaphore) == -1){
        perror("Cant post");
    }

    if(sem_close(my_semaphore) == -1){
        perror("Can't close the semaphore");
        exit(1);
    }

    if(sem_unlink(name) == -1){
        perror("Can't unlink the semaphore");
        exit(1);
    }

    exit(0);
}