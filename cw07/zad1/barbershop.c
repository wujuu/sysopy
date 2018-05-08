#include <time.h>
#include <stdio.h>
#include "barbershop.h"

void print_barbershop(struct barbershop *barbershop){
    printf("Barbershop stats:\n");

    if(barbershop -> state == 0){
        printf("Barber is sleeping\n");
    }else {
        printf("Barber is shaving\n");
    }
    printf("Max clients: %i\n", barbershop -> max_clients);
    printf("Current waiting clients amount: %i\n", barbershop -> queue.clients_amount);
    printf("Current waiting clients:\n");
    print_clients_queue(barbershop -> queue);


    printf("\n");
}

void print_time_msg(char *msg){
    clockid_t clk_id = CLOCK_MONOTONIC;
    struct timespec tp;
    clock_gettime(clk_id, &tp);

    printf("%lld.%.9ld: %s", (long long)tp.tv_sec, tp.tv_nsec, msg);
}