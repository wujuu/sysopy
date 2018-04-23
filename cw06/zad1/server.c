#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "client_server.h"


void mirror_handler(char* txt){
    char result[MSG_SIZE];

    for(int i = 0; i < MSG_SIZE; i++)
        result[i] = '\0';

    int last_index = 0;

    for(int i = 0; txt[i] != '\0'; i++){
        last_index = i;
    }

    int j = 0;
    for(int i = last_index; i >= 0; i--){
        result[j] = txt[i];
        j++;
    }

    return result;
}







int main(){
    key_t public_queue_key;
    int public_queue_id;

    public_queue_key = ftok(public_key_path, public_key_id);

    //Creating public message queue
    if((public_queue_id = msgget(public_queue_key, IPC_CREAT | IPC_EXCL | 0666)) < 0){
        perror("Failed to create a new queue!");
        exit(1);
    }




    //Removing public message queue
    if((msgctl(public_queue_id, IPC_RMID, NULL)) < 0){
        perror("Failed to remove the queue!");
        exit(1);
    }

    exit(0);
}