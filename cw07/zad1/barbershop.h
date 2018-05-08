#ifndef BARBERSHOP_H
#define BARBERSHOP_H


#include "clients_queue.h"

struct barbershop{
    int state; //0 - sleeping, 1 - shaving
    struct clients_queue queue;
    int max_clients;
    //int clients_waiting INSIDE CLIENTS QUEUE
    pid_t barber_pid;
    struct client passed_client;
};


void print_barbershop(struct barbershop *barbershop);

void print_time_msg(char *msg);

#endif