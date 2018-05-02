#ifndef SINGLE_SEM_H
#define SINGLE_SEM_H

#define PERMS 0666

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

struct semaphore{
    int id;
};

void remove_semaphore(struct semaphore *sem);

struct semaphore *init_semaphore(char *name);

void post_semaphore(struct semaphore *sem);

void wait_semaphore(struct semaphore *sem);

void print_sem_stats(struct semaphore *sem);

struct semaphore *open_semaphore(char* name);

#endif