#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "client_server.h"

int emergency = 0;

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

void delete_client(struct client *clients_array, int pid){
    int deleted_client_index = get_client_index(clients_array, pid);
    clients_array[deleted_client_index].pid = -1;
    clients_array[deleted_client_index].client_queue_id = -1;
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

void extract_txt(struct msg recieved_msg, char *txt_buffer){

    int txt_index = 0;

    for(int i = 0; i < MSG_SIZE && recieved_msg.txt[i] != ' ' && recieved_msg.txt[i] != '\0'; i++){
        txt_index = i;
    }

    txt_index += 2;

    int j = 0;
    for(int i = txt_index; i < MSG_SIZE && recieved_msg.txt[i] != '\0'; i++){
        txt_buffer[j] = recieved_msg.txt[i];
        j++;
    }
}






//Return messages handlers
struct msg get_mirror_return_msg(int sender_pid, char *message_txt){
    int last_index = 0;

    for(int i = 0; message_txt[i] != '\0'; i++){
        last_index = i;
    }

    char reverse_buffer[MSG_SIZE];
    fill_nulls(reverse_buffer);

    int j = 0;
    for(int i = last_index; i >= 0; i--){
        reverse_buffer[j] = message_txt[i];
        j++;
    }

    struct msg return_msg = init_msg(sender_pid, reverse_buffer);

    return return_msg;
}


struct msg get_start_return_msg(int sender_pid, char *message_txt, int failed){

    struct msg return_msg;

    if(failed){
        return_msg = init_msg(sender_pid, "No entry");
    } else {
        return_msg = init_msg(sender_pid, "Hello");
    }
    return return_msg;
}

struct msg get_time_return_msg(int sender_pid, char *message_txt){
    time_t rawtime;
    struct tm * timeinfo;

    time(&rawtime);
    timeinfo = localtime (&rawtime);
    char *h_read_time = asctime (timeinfo);

    remove_trailing_newline(h_read_time);

    struct msg return_msg = init_msg(sender_pid, h_read_time);

    return return_msg;
}


struct msg get_stop_return_msg(int sender_pid, char *message_txt){

    struct msg return_msg = init_msg(sender_pid, "Goodbye");

    return return_msg;
}

// void end_them_all(struct client *clients_array, int sender_pid){
//     int sender_index = get_client_index(clients_array, sender_pid);
//     struct msg stp_msg;

//     for(int i = 0; i < MAX_CLIENTS; i++){
//         if(clients_array[i].pid != -1 && i != sender_index){
//             stp_msg = get_stop_return_msg(clients_array[i].pid, "");
//             msgsnd(clients_array[i].client_queue_id, &stp_msg, MSG_SIZE, 0);
//         }
//     }

//     emergency = 1;
// }






void query_handler(struct msg recieved_msg, struct client *clients_array){
    //For storing return messages to the client
    struct msg return_msg;

    //Extracting sender's pid
    int sender_pid = extract_pid(recieved_msg);

    //Extracting recieved msg txt value
    char msg_txt[MSG_SIZE];
    fill_nulls(msg_txt);
    extract_txt(recieved_msg, msg_txt);

    if(recieved_msg.type == START){ 

        int client_queue_id = atoi(msg_txt);   

        if(get_free_index(clients_array) == -1){
            return_msg = get_start_return_msg(sender_pid, msg_txt, 1);
            msgsnd(client_queue_id, &return_msg, MSG_SIZE, 0);
            printf("Got a START message from %i, but MAX_CLIENTS exceeded\n", sender_pid);
            
        } else {
            insert_client(clients_array, sender_pid, client_queue_id);
            return_msg = get_start_return_msg(sender_pid, msg_txt, 0);
            printf("Got a START message from %i and starting the client\n", sender_pid);
        }
    }

    else if(recieved_msg.type == MIRROR){
        return_msg = get_mirror_return_msg(sender_pid, msg_txt);
        printf("Got a MIRROR message from %i\n", sender_pid);
    }

    else if(recieved_msg.type == TIME){  
        return_msg = get_time_return_msg(sender_pid, msg_txt);
        printf("Got a TIME message from %i\n", sender_pid);
    }

    else if(recieved_msg.type == STOP){
        return_msg = get_stop_return_msg(sender_pid, msg_txt);
        printf("Got a STOP message from %i\n", sender_pid);
    }

    else if(recieved_msg.type == END){
        return_msg = get_stop_return_msg(sender_pid, msg_txt);
        printf("Got an END message from %i\n", sender_pid);
        emergency = 1;
        // end_them_all(clients_array, sender_pid);
    }

    //Get client queue id
    int client_index = get_client_index(clients_array, sender_pid);
    int client_queue_id = clients_array[client_index].client_queue_id;

    //Send back return message
    msgsnd(client_queue_id, &return_msg, MSG_SIZE, 0);

    //Have to delete here, otherwise it wouldn't be able to communicate back
    if(recieved_msg.type == STOP) delete_client(clients_array, sender_pid);
}




int main(){
    struct client clients_array[MAX_CLIENTS];

    init_clients_array(clients_array);

    key_t public_queue_key;
    int public_queue_id;

    struct msg recieved_msg = init_msg(-1, "");

    public_queue_key = ftok(public_key_path, public_key_id);


    // Creating public message queue
    if((public_queue_id = msgget(public_queue_key, IPC_CREAT | 0666)) < 0){
        perror("Failed to create new public queue!");
        exit(1);
    }

    while(!emergency){
        msgrcv(public_queue_id, &recieved_msg, MSG_SIZE, 0, 0);

        query_handler(recieved_msg, clients_array);

        print_clients_array(clients_array);
    }

    // //Removing public message queue
    if((msgctl(public_queue_id, IPC_RMID, NULL)) < 0){
        perror("Failed to remove the public queue!");
        exit(1);
    }

    exit(0);
}