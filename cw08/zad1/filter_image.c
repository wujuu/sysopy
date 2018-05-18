#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include <unistd.h>

//GLOBAL ARRAYS AND SIZES FOR THREADS
int **input = NULL;
int **output = NULL;
int width = 0, height = 0;

double **filter = NULL;
int filter_size = 0;



struct thread_info{
    int start_index, end_index;
};



int max(int a, int b){
    if (a > b) return a;
    return b;
}

int min(int a, int b){
    if (a < b) return a;
    return b;
}

FILE *open_input(const char *file_name){
    FILE *fp = fopen(file_name, "r");
    if(fp == NULL){
        perror("Can't open a file");
    }

    return fp;
}

void close_input(FILE *fp){
    if(fclose(fp) != 0){
        perror("Can't close the file");
    }
}

void read_pgm(const char* file_name, int ***output_table, int *output_width, int *output_height){
    FILE *fp = open_input(file_name);
    char *buff = NULL;
    size_t len = 0;

    //P2 TOKEN
    getline(&buff, &len, fp);

    //WIDTH AND height
    getline(&buff, &len, fp);

    *output_width = atoi(strsep(&buff, " "));
    *output_height = atoi(strsep(&buff, " "));

    // printf("%i %i\n", *output_width, *output_height);

    //ALLOCATING PROPER OUTPUT TABLE
    *output_table = malloc((*output_height) * sizeof(int*));
    for(int i = 0; i < (*output_height); i++){
        (*output_table)[i] = malloc((*output_width) * sizeof(int));
    }

    //MAX VALUE OF
    getline(&buff, &len, fp);

    // printf("%s\n", buff);

    //LINES
    int i = 0, j = 0;
    char *tmp;

    while(getline(&buff, &len, fp) != -1){
        while((tmp = strsep(&buff, " "))){
            if(strcmp(tmp, "") != 0 && strcmp(tmp, "\r\n") && strcmp(tmp, "\n")!= 0){
                (*output_table)[i][j] = atoi(tmp);
                j = (j + 1) % (*output_width);
                if (j == 0) i++;
            }
        }
    }


    free(buff);

    close_input(fp);
}

void read_filter(const char* file_name, double ***filter_table, int *filter_size){
    FILE *fp = open_input(file_name);
    char *buff = NULL;
    size_t len = 0;

    //FILTER SIZE
    getline(&buff, &len, fp);
    (*filter_size) = atoi(buff);

    //ALLOCATING PROPER FILTER SIZE
    *filter_table = malloc((*filter_size) * sizeof(double*));
    for(int i = 0; i < (*filter_size); i++){
        (*filter_table)[i] = malloc((*filter_size) * sizeof(double));
    }

    //LINES
    int i = 0, j = 0;
    char *tmp;

    while(getline(&buff, &len, fp) != -1){
        while((tmp = strsep(&buff, " "))){
            if(strcmp(tmp, "") != 0 && strcmp(tmp, "\n") != 0){
                (*filter_table)[i][j] = atof(tmp);
                j = (j + 1) % (*filter_size);
                if (j == 0) i++;
            }
        }
    }

    //CLEANING UP
    free(buff);

    close_input(fp);
}

void *process_chunk(void *void_info){
    //CASTING TO A MEANIGFUL STRUCTURE
    struct thread_info *info = (struct thread_info*) void_info;

    //SPECIAL VALUE USED FOR FILTERING
    int tmp = (int) ceil(filter_size/2);

    //FILTERING
    for(int i = info -> start_index; i < info -> end_index; i++){
        for(int j = 0; j < width; j++){

            double filter_val = 0;

            for(int k = 0; k < filter_size; k++){
                for(int l = 0; l < filter_size; l++){

                    int x = min(height - 1, max(0, i - tmp + k));
                    int y = min(width- 1, max(0, j - tmp + l));

                    filter_val += ((double) input[x][y]) * filter[k][l];
                }
            }

            output[i][j] = min(255, max(0, (int) round(filter_val)));
        }
    }

    return NULL;
}

void write_to_file(const char *file_name){
    //CREATING A FILE
    FILE *fp = fopen(file_name, "w");

    //ADDING HEADER INFOS
    fprintf(fp, "P2\n");

    fprintf(fp, "%i %i\n", width, height);

    fprintf(fp, "255\n");

    //ALLOCATING BUFFERS
    char line[128];
     for(int k = 0; k < 128; k++) line[k] = '\0';
    char token[5];

    //MAX 17 TOKENS PER LINE AS IN PGM SPECIFICATION(70 CHARS PER LINE)
    int token_counter = 0;

    //WRITING TO OUTPUT FILE
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){

            sprintf(token, "%d ", output[i][j]);

            strcat(line, token);

            token_counter++;

            //WRITING WHOLE LINE TO FILE
            if(token_counter == 17){
                fprintf(fp, "%s\n", line);

                //RESETING LINE BUFFER
                for(int k = 0; k < 128; k++) line[k] = '\0';

                token_counter = 0;
            }

        }
    }


    fclose(fp);
}

struct thread_info *divide_work(int threads_amount){
    //ALLOCATING THREAD INFO ARRAY
    struct thread_info *thread_infos = malloc(threads_amount * sizeof(struct thread_info));

    //CALUCLATING CHUNK SIZE
    int chunk_size = height / threads_amount;
    int prev_end = 0;

    //DIVING CHUNKS
    for(int i = 0; i < threads_amount; i++){
        int start_index = prev_end;
        int end_index = start_index + chunk_size;

        thread_infos[i].start_index = start_index;
        thread_infos[i].end_index = end_index;

        prev_end = end_index;
    }

    //LAST THREAD HAS TO FILL TO THE END OF THE ARRAY IF NOT EVENLY DISTRIBUTED
    if(prev_end < height){
        thread_infos[threads_amount - 1].end_index = height;
    }

    return thread_infos;
}

int main(int argc, char **argv){

    //READING USER INPUT
    int threads_amount = atoi(argv[1]);
    const char *input_file = argv[2];
    const char *filter_file = argv[3];
    const char *output_file = argv[4];

    //READING INPUT AND FILTER FILES
    read_pgm(input_file, &input, &width, &height);
    read_filter(filter_file, &filter, &filter_size);

    //ALLOCATING OUTPUT
    output = malloc(height * sizeof(int*));

    for(int i = 0; i < height; i++)
        output[i] = malloc(width * sizeof(int));

    //ALLOCATING THREAD ID ARRAY
    pthread_t *threads = malloc(threads_amount * sizeof(pthread_t));

    //DIVIDING WORK
    struct thread_info *thread_infos = divide_work(threads_amount);


    //SCHDULING THREADS
    for(int i = 0; i < threads_amount; i++)
        pthread_create(&threads[i], NULL, &process_chunk, &thread_infos[i]);


    //WAITING FOR THREADS TO JOIN THE MAIN THREAD
    for(int i = 0; i < threads_amount; i++)
        pthread_join(threads[i], NULL);


    // WRITING TO OUTPUT FILE
    write_to_file(output_file);


    // CLEANING UP BUFFERS
    // for(int i = 0; i < height; ++i){
    //     free(output[i]);
    // }

    // free(output);

    // for(int i = 0; i < threads_amount; ++i){
    //     free(&threads[i]);
    //     free(&thread_infos[i]);
    // }

    exit(0);
}