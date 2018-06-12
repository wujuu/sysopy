#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int x = -1, y = 1;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond   = PTHREAD_COND_INITIALIZER;

void *thread_func1(void *arg){
    pthread_mutex_lock(&mutex);

    while(x <= y)
        pthread_cond_wait(&cond, &mutex);

    printf("Wreszcie %i jest wieksze od %i\n", x, y);

    pthread_mutex_unlock(&mutex);
}


void *thread_func2(void *arg){
    while(x <= y){

        printf("nie nie, jeszcze nie\n");

        pthread_mutex_lock(&mutex);

        x = rand() % 100;
        y = rand() % 100;

        if(x > y){
            pthread_cond_broadcast(&cond);
        }
            
        pthread_mutex_unlock(&mutex);

        sleep(1);
    }
}




int main(){
    srand(time(NULL));
    



    pthread_mutex_init(&mutex, NULL);

    pthread_cond_init(&cond, NULL);

    pthread_t t1, t2;
    pthread_create(&t1, NULL, thread_func1, NULL);
    pthread_create(&t2, NULL, thread_func2, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);


    exit(0);
}