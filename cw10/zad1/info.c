#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <pthread.h>
#include <sys/select.h>
#include <unistd.h>
#include <signal.h>
#include "info.h"

char *to_upper_case(char* string){
    char* upper_string = malloc(strlen(string));
    strcpy(upper_string, string);

    for(int i = 0; i < strlen(string); i++) upper_string[i] = toupper(string[i]);

    return upper_string;
}

int is_valid_num(char* string){
    for(int i = 0; i < strlen(string); i++) 
        if ((string[i] < '0' || string[i] > '9') && (string[i] != '\n'))
            return 0;

    return 1;
}

struct task *parse_task(char* input){
    char* upper_input = to_upper_case(input);
    char *tmp = NULL;

    struct task *new_task = malloc(sizeof(struct task));


    //TASK STRING       
    tmp = strsep(&upper_input, " ");
    if(strcmp(tmp, "ADD") == 0 || strcmp(tmp, "SUB") == 0 || strcmp(tmp, "MUL") == 0 || strcmp(tmp, "DIV") == 0)
        strcpy(new_task -> task_name, tmp);
    else
        return NULL;
    
    //X OPERAND
    tmp = strsep(&upper_input, " ");
    if(is_valid_num(tmp))
        new_task -> x = atoi(tmp);
    else
        return NULL;

    //Y OPERAND
    tmp = strsep(&upper_input, " ");
    if(is_valid_num(tmp))
        new_task -> y = atoi(tmp);
    else
        return NULL;

    
    return new_task;
}

void print_task(struct task *input_task){
    if(input_task != NULL)
        printf("task: %s, x: %i, y: %i\n", input_task -> task_name, input_task -> x, input_task -> y);
}

void task_to_string(struct task *input_task, char *task_buff){
    for(int i = 0; i < strlen(task_buff); i++) task_buff[i] = 0;
    
    strcat(task_buff,  input_task -> task_name);
    strcat(task_buff, " ");

    char str[10];
    sprintf(str, "%d", input_task -> x);

    strcat(task_buff,  str);
    strcat(task_buff, " ");

    sprintf(str, "%d", input_task -> y);
    strcat(task_buff,  str);
}
