#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <math.h>

#define BUFF_SIZE 256

void close_input(FILE *input_fp){
    if(fclose(input_fp) != 0){
        perror("Can't close the file");
    }
}

FILE *open_input(const char *file_name){
    FILE *input_fp = fopen(file_name, "r");
    if(input_fp == NULL){
        perror("Can't open a file");
    }

    return input_fp;
}

void **get_table(const char* file_name, float ***output_table, int *output_width, int *output_length){
    FILE *input_fp = NULL;
    char *buff = NULL;
    size_t len = 0;
    int width = 0, length = 0, max_val = 0;

    input_fp = open_input(file_name);

    getline(&buff, &len, input_fp);

    getline(&buff, &len, input_fp);

    width = atoi(strsep(&buff, " "));
    length = atoi(strsep(&buff, " "));

    *output_width = width;
    *output_length = length;

    getline(&buff, &len, input_fp);

    max_val = atoi(buff);

    *output_table = malloc(length * sizeof(float*));
    for(int i = 0; i < length; i++){
        (*output_table)[i] = malloc(width * sizeof(float));
    }


    //READ LINES
    int i = 0, j = 0;
    char *tmp;

    while(getline(&buff, &len, input_fp) != -1){
        while((tmp = strsep(&buff, " "))){
            (*output_table)[i][j] = atof(tmp);
            i = (i + 1) % length;
            if (i == 0) j++;
        }
    }
    //END READ LINES


    //CLEANING UP
    if(buff != NULL){
        free(buff);
    }


    close_input(input_fp);
}

void free_table(float ***input_table, int length){
    for(int i = 0; i < length; i++){
        free( (*input_table)[i] );
    }

    // free(*input_table);
}

void print_table(float **input_table, int width, int length){
    for(int i = 0; i < length; i++){
        for(int j = 0; j < width; j++){
            printf("%f ", input_table[i][j]);
        }
        printf("\n");
    }
}



void cast_to_int(int ***output_table, float **input_table, int input_width, int input_length){
    *output_table = malloc(input_length * sizeof(int*));
    for(int i = 0; i < input_length; i++){
        (*output_table)[i] = malloc(input_width * sizeof(int));
    }

    for(int i = 0; i < input_length; i++){
        for(int j = 0; j < input_width; j++){
            (*output_table)[i][j] = floor(input_table[i][j]);
        }
    }
}

// for(int i = 0; )



int main(){

    float **input = NULL, **filter = NULL;
    int input_width = 0, input_length = 0, filter_width = 0, filter_length = 0;

    get_table("body3.ascii.pgm", &input, &input_width, &input_length);
    get_table("filter.ascii.pgm", &filter, &filter_width, &filter_length);

    int **int_input = NULL,  **output = NULL;

    output = malloc(input_length * sizeof(int*));

    for(int i = 0; i < input_length; i++){
        for(int j = 0; j < input_width; j++){
            output[i] = malloc(input_length * sizeof(int));
        }
    }

    cast_to_int(&int_input, input, input_width, input_length);

    for(int i = 0; i < input_length; i++){
        for(int j = 0; j < input_width; j++){

            float filter_val = 0;

            for(int k = 0; k < filter_length; k++){
                for(int l = 0; l < filter_width; l++){
                    int x = fmax(1.0, i - ceil(filter_length/2) + k);
                    int y = fmax(1.0, j - ceil(filter_length/2) + l);
                    filter_val += (input[x][y] * filter[k][l]);
                }
            }
            output[i][j] = (int) round(filter_val);
        }
    }

    // for(int i = 0; i < input_length; i++){
    //     for(int j = 0; j < input_width; j++){
    //         printf("%i ", output[i][j]);
    //     }
    //     printf("\n");
    // }



    // print_table(int_input, input_width, input_length);
    // print_table(filter, filter_width, filter_length);


    // free_table(&input, input_length);
    // free_table(&filter, filter_length);

    exit(0);
}