#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>
#include "client_server.h"

struct msg make_start_msg(int private_queue_id){
    int client_pid = getpid();
    char str_client_pid[MSG_SIZE];
    snprintf(str_client_pid, MSG_SIZE, "%d", client_pid);

    struct msg new_start_msg = init_msg(START, str_client_pid);

    char str_private_queue_id[MSG_SIZE];
    snprintf(str_private_queue_id, MSG_SIZE, "%d", private_queue_id);

    strcat(new_start_msg.txt, " ");

    strcat(new_start_msg.txt, str_private_queue_id);

    return new_start_msg;
}

struct msg make_mirror_msg(char *new_txt){
    int client_pid = getpid();
    char str_client_pid[MSG_SIZE];
    snprintf(str_client_pid, MSG_SIZE, "%d", client_pid);

    strcat(str_client_pid, " ");
    strcat(str_client_pid, new_txt);

    struct msg new_mirror_msg = init_msg(MIRROR, str_client_pid);

    return new_mirror_msg;
}



int main(){


    key_t public_queue_key;
    int public_queue_id;

    public_queue_key = ftok(public_key_path, public_key_id);

    //Opening public message queue
    if((public_queue_id = msgget(public_queue_key, 0000)) < 0){
        perror("Failed to open a public queue!");
        exit(1);
    }

    int private_queue_id;

    if((private_queue_id = msgget(IPC_PRIVATE, IPC_CREAT | IPC_EXCL | 0666)) < 0){
        perror("Failed to create a private queue!");
        exit(1);
    }


    struct msg start_msg = make_start_msg(private_queue_id);
    struct msg mirror_msg = make_mirror_msg("Kozlak to chuj");
    struct msg rcv_msg_buffer = init_msg(0, "");

    if(msgsnd(public_queue_id, &start_msg, MSG_SIZE, 0) < 0){
        perror("Failed to send a message queue!");
        exit(1);
    }

    if(msgrcv(private_queue_id, &rcv_msg_buffer, MSG_SIZE, 0, 0)){
        perror("Failed to recieve a message queue!");
        exit(1);
    }


    if(msgsnd(public_queue_id, &mirror_msg, MSG_SIZE, 0) < 0){
        perror("Failed to send a message queue!");
        exit(1);
    }


    if(msgrcv(private_queue_id, &rcv_msg_buffer, MSG_SIZE, 0, 0)){
        perror("Failed to recieve a message queue!");
        exit(1);
    }

    printf("%s\n", rcv_msg_buffer.txt);




    // print_queue_stats(private_queue_id);



    exit(0);
}