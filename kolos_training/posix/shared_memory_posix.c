#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

struct shared_struct{
    int x, y;
    char wtf;
};


int main(){


    const char name[] = "/shared_mem1";

    off_t shared_struct_size = sizeof(struct shared_struct);

    int fd_shm = -1;

    fd_shm = shm_open(name, O_CREAT | O_RDWR, S_IRWXU);

    if(fd_shm == -1){
        perror("cant create shm");
        exit(1);
    }

    if(ftruncate(fd_shm, shared_struct_size) == -1){
        perror("Can't ftruncate");
    }

    //write attempt
    void *shm_addr = mmap(NULL, shared_struct_size, PROT_WRITE, MAP_SHARED, fd_shm, 0);

    int mmap_return = *((int *) shm_addr);

    if( mmap_return == -1){
        perror("can't attach");
    }

    struct shared_struct *got_shm_struct = (struct shared_struct *) shm_addr; 

    got_shm_struct -> x = 10;
    got_shm_struct -> y = 20;
    got_shm_struct -> wtf = 'w';

    if(munmap(shm_addr, shared_struct_size) == -1){
        perror("can't unnatach");
    }

    //waiting for another process to write something else instead
    char txt[10];
    scanf("%s", txt);

    //read attepmpt
    shm_addr = mmap(NULL, shared_struct_size, PROT_READ, MAP_SHARED, fd_shm, 0);

    mmap_return = *((int *) shm_addr);

    if( mmap_return == -1){
        perror("can't attach");
    }

    got_shm_struct = (struct shared_struct *) shm_addr; 

    printf("%i\n", got_shm_struct -> x);
    printf("%i\n", got_shm_struct -> y);
    printf("%c\n", got_shm_struct -> wtf);

    if(munmap(shm_addr, shared_struct_size) == -1){
        perror("can't unnatach");
    }

    
    if(shm_unlink(name) == -1){
        perror("Can't unlink");
        exit(1);
    }

    exit(0);
}