#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int protected_int = 0;

pthread_mutex_t new_mutex = PTHREAD_MUTEX_INITIALIZER;

void *thread_func1(void *arg){
    pthread_mutex_lock(&new_mutex);

    pthread_t my_id = pthread_self();
    printf("I'm %li and im using func1 and ", my_id);
    printf("My version of protected_int is: %i\n", protected_int);
    protected_int = 1;
    pthread_mutex_unlock(&new_mutex);
}

void *thread_func2(void *arg){
    pthread_mutex_lock(&new_mutex);

    pthread_t my_id = pthread_self();
    printf("I'm %li and im using func2 and ", my_id);
    printf("My version of protected_int is: %i\n", protected_int);
    protected_int = 2;
    pthread_mutex_unlock(&new_mutex);
}


int main(){
    

    pthread_t t1, t2;
    // pthread_mutexattr_t new_mutex_attr;

    pthread_mutex_init(&new_mutex, NULL);

    pthread_create(&t1, NULL, thread_func1, NULL);
    pthread_create(&t2, NULL, thread_func2, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);


    exit(0);
}