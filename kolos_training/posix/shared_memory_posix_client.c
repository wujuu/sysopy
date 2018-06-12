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

    fd_shm = shm_open(name, O_RDWR, S_IRWXU);

    if(fd_shm == -1){
        perror("cant open shm");
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

    got_shm_struct -> x = 40;
    got_shm_struct -> y = 100;
    got_shm_struct -> wtf = 'e';

    if(munmap(shm_addr, shared_struct_size) == -1){
        perror("can't unnatach");
    }

    exit(0);
}