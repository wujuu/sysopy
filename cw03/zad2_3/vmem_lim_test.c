#include <stdlib.h>


int main(){

    char *array = malloc(1024 * 1000000);

    array[0] = 'a';

    exit(0);
}