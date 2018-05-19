#ifndef FILTER_IMAGE_H
#define FILTER_IMAGE_H

#include <stdio.h>


struct thread_info{
    int start_index, end_index;
};

int max(int a, int b);

int min(int a, int b);

FILE *open_input(const char *file_name);

void close_input(FILE *fp);

void read_pgm(const char* file_name, int ***output_table, int *output_width, int *output_height);

void read_filter(const char* file_name, double ***filter_table, int *filter_size);

void *process_chunk(void *void_info);

void write_to_file(const char *file_name);

struct thread_info *divide_work(int threads_amount);

void filter_image(int threads_amount, const char *input_file, const char *filter_file, const char *output_file);

#endif