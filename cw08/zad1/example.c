#include <stdio.h>
#include <stdlib.h>
#include "filter_image.h"

int main(int argc, char **argv){

    //READING USER INPUT
    int threads_amount = atoi(argv[1]);
    const char *input_file = argv[2];
    const char *filter_file = argv[3];
    const char *output_file = argv[4];

    filter_image(threads_amount, input_file, filter_file, output_file);

    exit(0);
}