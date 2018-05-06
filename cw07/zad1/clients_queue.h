#ifndef CLIENTS_QUEUE_H
#define CLIENTS_QUEUE_H

#include <sys/types.h>

#define MAX_CLIENTS_LIMIT 100

struct client{
    pid_t pid;
    int times_cut;
    int times_to_cut;
};

struct clients_queue{
    struct client clients_array[MAX_CLIENTS_LIMIT];
    int head_index;
    int last_index;
    int clients_amount;
};


struct client init_client(pid_t pid, int times_to_cut);

struct clients_queue init_clients_queue();

int is_empty(struct clients_queue queue);

void print_client(struct client c);

void print_clients_queue(struct clients_queue queue);

void push_clients_queue(struct clients_queue *queue, struct client new_client);

struct client pop_clients_queue(struct clients_queue *queue);

#endif