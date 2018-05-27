#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>

int P = 0, K = 0, N = 0, L = 0, verbose = 0, find_mode = 0, nk = 0;
char *input_file_path = NULL;

char **buffer = NULL;
FILE *input_fp = NULL;

int prev_write_index = 0;
int prev_read_index = 0;


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

void read_config_file(char* config_file_path){
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

int main(int argc, char **argv){

    read_config_file(argv[1]);

    allocate_buffer();

    input_fp = fopen(input_file_path, "r");

    
    //MAIN KURWA JA PIERDOLE LOOP


    fclose(input_fp);

    exit(0);
}