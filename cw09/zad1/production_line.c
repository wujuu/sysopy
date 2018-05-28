#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

int P = 0, K = 0, N = 0, L = 0, verbose = 0, find_mode = 0, nk = 0;
char *input_file_path = NULL;

char **buffer = NULL;
FILE *input_fp = NULL;

int prev_write_index = 0;
int prev_read_index = 0;

sem_t *empty_sem = NULL;
sem_t *full_sem = NULL;
sem_t *buffer_sem = NULL;



void allocate_buffer(){
    buffer = malloc(N * sizeof(char*));

    for(int i = 0; i < N; i++) 
        buffer[i] = NULL;
}

void display_buffer(){
    for(int i = 0; i < N; i++) 
        if(buffer[i] != NULL) printf("%i: %s", i, buffer[i]);
        else printf("%i: empty\n", i);

    printf("\n");
}

int is_empty(){
    for(int i = 0; i < N; i++)
        if(buffer[i] != NULL) 
            return 0;
    
    return 1;
}

int is_full(){
    for(int i = 0; i < N; i++)
        if(buffer[i] == NULL) 
            return 0;
    
    return 1;
}

void write_to_buffer(){
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    read = getline(&line, &len, input_fp);

    buffer[prev_write_index] = line;

    prev_write_index++;
    prev_write_index %= N;
}

char* read_from_buffer(){
    char* contents = buffer[prev_read_index];

    buffer[prev_read_index] = NULL;

    prev_read_index++;
    prev_read_index %= N;

    return contents;
}

void read_config_file(char *config_file_path){
    FILE *config_fp = fopen(config_file_path, "r");
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    read = getline(&line, &len, config_fp);
    P = atoi(line);

    read = getline(&line, &len, config_fp);
    K = atoi(line);

    read = getline(&line, &len, config_fp);
    N = atoi(line);

    read = getline(&line, &len, config_fp);
    input_file_path = malloc(sizeof(line) + 1);
    strcpy(input_file_path, line);
    input_file_path[strcspn(input_file_path, "\n")] = 0;

    read = getline(&line, &len, config_fp);
    L = atoi(line);

    read = getline(&line, &len, config_fp);
    find_mode = atoi(line);

    read = getline(&line, &len, config_fp);
    verbose = atoi(line);

    read = getline(&line, &len, config_fp);
    nk = atoi(line);

    free(line);

    fclose(config_fp);
}

void sigint_handler(int sig){
    sem_close(empty_sem);
    sem_close(full_sem);
    sem_close(buffer_sem);
    fclose(input_fp);
    exit(0);
}


void *producer_function(void *arg){

    sem_wait(buffer_sem);

    int full_sem_val = -1;
    sem_getvalue(full_sem, &full_sem_val);

    if(is_full() && full_sem_val != 0)
        sem_wait(full_sem);

    sem_post(buffer_sem);

    sem_wait(full_sem);
    sem_post(full_sem);

    sem_wait(buffer_sem);

    write_to_buffer();

    sem_post(buffer_sem);


    int empty_sem_val = -1;
    sem_getvalue(empty_sem, &empty_sem_val);

    if(empty_sem_val == 0)
        sem_post(empty_sem);


    return NULL;
}

void *consumer_function(void *arg){

    sem_wait(buffer_sem);

    int empty_sem_val = -1;
    sem_getvalue(empty_sem, &empty_sem_val);

    if(is_empty() && empty_sem_val != 0)
        sem_wait(empty_sem);

    sem_post(buffer_sem);

    sem_wait(empty_sem);
    sem_post(empty_sem);

    sem_wait(buffer_sem);

    char *string_read = read_from_buffer();
    printf("%s", string_read);

    sem_post(buffer_sem);

    int full_sem_val = -1;
    sem_getvalue(full_sem, &full_sem_val);

    if(full_sem_val == 0)
        sem_post(full_sem);

    
    return NULL;
}





int main(int argc, char **argv){

    signal(SIGINT, sigint_handler);

    read_config_file(argv[1]);

    allocate_buffer();


    empty_sem = sem_open("empty",  O_RDWR |  O_CREAT, 0666, 1);
    full_sem = sem_open("full",  O_RDWR |  O_CREAT, 0666, 1);
    buffer_sem = sem_open("buffer",  O_RDWR |  O_CREAT, 0666, 1);


    input_fp = fopen(input_file_path, "r");

    pthread_t *producer_threads = malloc(P * sizeof(pthread_t));
    pthread_t *consumer_threads = malloc(K * sizeof(pthread_t));

    //MAIN LOOP

    while(1){
        for(int i = 0; i < P; i++)
            pthread_create(&producer_threads[i], NULL, &producer_function, NULL);

        for(int i = 0; i < K; i++)
            pthread_create(&consumer_threads[i], NULL, &consumer_function, NULL);

        for(int i = 0; i < P; i++)
            pthread_join(producer_threads[i], NULL);

        for(int i = 0; i < K; i++)
            pthread_join(consumer_threads[i], NULL);
    }
    

    exit(0);
}