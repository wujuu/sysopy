#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>
#include "client_server.h"

struct client{
    int pid;
    int client_queue_id;
};

int get_client_index(struct client *clients_array, int pid){
    for(int i = 0; i < MAX_CLIENTS; i++){
        if(clients_array[i].pid == pid){
            return i;
        }
    }
    return -1;
}

void init_clients_array(struct client *clients_array){

    for(int i = 0; i < MAX_CLIENTS; i++){
        clients_array[i].client_queue_id = -1;
        clients_array[i].pid = -1;
    }

}

int get_free_index(struct client *clients_array){
    for(int i = 0; i < MAX_CLIENTS; i++){
        if(clients_array[i].pid == -1){
            return i;
        }
    }
    return -1;
}

void insert_client(struct client *clients_array, int pid, int client_queue_id){
    int free_index = get_free_index(clients_array);
    clients_array[free_index].pid = pid;
    clients_array[free_index].client_queue_id = client_queue_id; 
}

void print_clients_array(struct client *clients_array){
    for(int i = 0; i < MAX_CLIENTS; i++){
        printf("index: %i, pid: %i, queue_id: %i\n", i, clients_array[i].pid, clients_array[i].client_queue_id);
    }
}





int extract_pid(struct msg recieved_msg){
    char pid_buffer[MSG_SIZE];

    fill_nulls(pid_buffer);

    for(int i = 0; i < MSG_SIZE && recieved_msg.txt[i] != ' ' && recieved_msg.txt[i] != '\0'; i++)
        pid_buffer[i] = recieved_msg.txt[i];
    
    return atoi(pid_buffer);
}


struct msg mirror_hanler(struct msg recieved_msg){
    int sender_pid = extract_pid(recieved_msg);

    struct msg return_msg = init_msg(sender_pid, "");

    int first_index = 0;

    for(int i = 0; i < MSG_SIZE && recieved_msg.txt[i] != ' '; i++)
        first_index = i;

    first_index++;

    int last_index = 0;

    for(int i = 0; recieved_msg.txt[i] != '\0'; i++){
        last_index = i;
    }

    int j = 0;
    for(int i = last_index; i > first_index; i--){
        return_msg.txt[j] = recieved_msg.txt[i];
        j++;
    }

    return return_msg;
}


struct msg start_handler(struct msg recieved_msg){
    int pid = extract_pid(recieved_msg);
    char str_client_queue_id[MSG_SIZE];
    fill_nulls(str_client_queue_id);

    int queue_index = 0;

    for(int i = 0; i < MSG_SIZE && recieved_msg.txt[i] != ' '; i++){
        queue_index = i;
    }
    queue_index++;

    int j = 0;
    for(int i = queue_index; i < MSG_SIZE && recieved_msg.txt[i] != '\0'; i++){
        str_client_queue_id[j] = recieved_msg.txt[i];
        j++;
    }


    struct msg return_msg = init_msg(pid, str_client_queue_id);

    return return_msg;
}


void query_handler(struct msg recieved_msg, struct client *clients_array){
    struct msg return_msg;
    
    if(recieved_msg.type == START){
        return_msg = start_handler(recieved_msg);

        insert_client(clients_array, return_msg.type, atoi(return_msg.txt));

        printf("Got a start message from %i\n", return_msg.type);
    }
    if(recieved_msg.type == MIRROR){
        
        return_msg = mirror_hanler(recieved_msg);

        printf("Got a mirror message from %i\n", return_msg.type);
    }

    int client_index = get_client_index(clients_array, return_msg.type);

    msgsnd(clients_array[client_index].client_queue_id, &return_msg, MSG_SIZE, 0);
}




int main(){

    struct client clients_array[MAX_CLIENTS];

    init_clients_array(clients_array);

    key_t public_queue_key;
    int public_queue_id;

    struct msg recieved_msg = init_msg(-1, "");

    public_queue_key = ftok(public_key_path, public_key_id);


    // Creating public message queue
    if((public_queue_id = msgget(public_queue_key, IPC_CREAT | 0666)) < 0){ //IPC_CREAT | IPC_EXCL | 0666
        perror("Failed to create new public queue!");
        exit(1);
    }

    while(1){
        msgrcv(public_queue_id, &recieved_msg, MSG_SIZE, 0, 0);

        query_handler(recieved_msg, clients_array);
    }

    // //Removing public message queue
    if((msgctl(public_queue_id, IPC_RMID, NULL)) < 0){
        perror("Failed to remove the public queue!");
        exit(1);
    }

    exit(0);
}