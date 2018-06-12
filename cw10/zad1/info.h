#ifndef FILTER_IMAGE_H
#define FILTER_IMAGE_H

#define MAX_CLIENTS 4
#define MAX_CLIENT_NAME_LENGTH 8

#define MAX_TASK_NAME_LENGTH 16
#define MAX_TASK_LENGTH 256
#define MAX_TASKS 16



struct client{
    char name[MAX_CLIENT_NAME_LENGTH];
    int socket_fd;
};

struct task{
    char task_name[MAX_TASK_NAME_LENGTH];
    int x, y;
};


char *to_upper_case(char* string);

int is_valid_num(char* string);

struct task *parse_task(char* input);

void print_task(struct task *input_task);

void task_to_string(struct task *input_task, char *task_buff);


#endif