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


void start_handler(struct msg recieved_msg, struct client *clients_array){
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

    int client_queue_id = atoi(str_client_queue_id);

    insert_client(clients_array, pid, client_queue_id);

    


}

struct msg query_handler(struct msg recieved_msg){
    int sender_pid = extract_pid(recieved_msg);
    
    if(recieved_msg.type == START){

    }
    if(recieved_msg.type == MIRROR){
        return mirror_hanler(recieved_msg);
    }
}



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



int main(){

    struct client clients_array[MAX_CLIENTS];

    init_clients_array(clients_array);

    struct msg start_msg = make_start_msg(12);

    start_handler(start_msg, clients_array);

    print_clients_array(clients_array);


    // key_t public_queue_key;
    // int public_queue_id;

    // public_queue_key = ftok(public_key_path, public_key_id);

    // //Creating public message queue
    // if((public_queue_id = msgget(public_queue_key, IPC_CREAT | IPC_EXCL | 0666)) < 0){
    //     perror("Failed to create new public queue!");
    //     exit(1);
    // }




    // //Removing public message queue
    // if((msgctl(public_queue_id, IPC_RMID, NULL)) < 0){
    //     perror("Failed to remove the public queue!");
    //     exit(1);
    // }

    exit(0);
}