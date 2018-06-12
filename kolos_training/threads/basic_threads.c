#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void *thread_func(void *arg){
    printf("hehehszki ");
    pthread_t my_id = pthread_self();
    printf("to ja %li\n", my_id);
}


int main(){
    pthread_t new_thread_id;

    pthread_create(&new_thread_id, NULL, thread_func, NULL);
    
    printf("to on %li\n", new_thread_id);

    pthread_join(new_thread_id, NULL);

    exit(0);
}