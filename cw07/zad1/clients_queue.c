#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include "clients_queue.h"

struct client init_client(pid_t pid, int times_to_cut){
    struct client new_client;

    new_client.pid = pid;
    new_client.times_cut = 0;
    new_client.times_to_cut = times_to_cut;

    return new_client;
}

struct clients_queue init_clients_queue(){
    struct clients_queue new_queue;

    new_queue.head_index = 0;
    new_queue.last_index = 0;

    new_queue.clients_amount = 0;

    return new_queue;
}

int is_empty(struct clients_queue queue){
    if(queue.clients_amount == 0)
        return 1;
    return 0;
}

void print_client(struct client c){
    printf("- PID of the client: %i, ", c.pid);
    printf("Times cut: %i, ", c.times_cut);
    printf("Times to cut: %i\n", c.times_to_cut);
}

void print_clients_queue(struct clients_queue queue){
    if(!is_empty(queue)){
        for(int i = queue.head_index; i <= queue.last_index; i++){
            print_client(queue.clients_array[i]);
        }
    }
}


void push_clients_queue(struct clients_queue *queue, struct client new_client){
    if(is_empty(*queue)){
        queue -> clients_array[queue -> head_index] = new_client;
    } else {
        if(queue -> last_index + 1 >= MAX_CLIENTS_LIMIT){
            struct client tmp_clients_array[MAX_CLIENTS_LIMIT];

            int j = 0;

            for(int i = queue -> head_index; i < MAX_CLIENTS_LIMIT; i++){
                tmp_clients_array[j] = queue -> clients_array[i];
                j++;
            }

            for(int i = 0; i < MAX_CLIENTS_LIMIT; i++){
                queue -> clients_array[i] = tmp_clients_array[i];
            }

            queue -> head_index = 0;
            queue -> last_index = queue -> head_index + queue -> clients_amount - 1;
        }
        queue -> last_index++;
        queue -> clients_array[queue -> last_index] = new_client;
    }
    queue -> clients_amount++;
}

struct client pop_clients_queue(struct clients_queue *queue){

    if(!is_empty(*queue)){
        int pop_index = queue -> head_index;

        if(queue -> clients_amount != 1){
            queue -> head_index++;
        }

        queue -> clients_amount--;

        return queue -> clients_array[pop_index];
    } else {
        printf("Can't pop from empty list!\n");
    }
}
