#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "client_server.h"



int main(){

    key_t public_queue_key;
    int public_queue_id;

    public_queue_key = ftok(public_key_path, public_key_id);

    //Opening public message queue
    if((public_queue_id = msgget(public_queue_key, 0000)) < 0){
        perror("Failed to create a new queue!");
        exit(1);
    }


    exit(0);
}