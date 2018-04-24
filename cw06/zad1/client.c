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

    char str_private_queue_id[MSG_SIZE];
    snprintf(str_private_queue_id, MSG_SIZE, "%d", private_queue_id);

    char new_msg_txt[MSG_SIZE];
    fill_nulls(new_msg_txt);

    strcpy(new_msg_txt, str_client_pid);

    strcat(new_msg_txt, " ");

    strcat(new_msg_txt, str_private_queue_id);

    struct msg new_start_msg = init_msg(START, new_msg_txt);

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

struct msg make_time_msg(){
    int client_pid = getpid();
    char str_client_pid[MSG_SIZE];
    snprintf(str_client_pid, MSG_SIZE, "%d", client_pid);

    struct msg new_time_msg = init_msg(TIME, str_client_pid);

    return new_time_msg;
}

struct msg make_stop_msg(){
    int client_pid = getpid();
    char str_client_pid[MSG_SIZE];
    snprintf(str_client_pid, MSG_SIZE, "%d", client_pid);

    struct msg new_stop_msg = init_msg(STOP, str_client_pid);
    
    return new_stop_msg;
}

struct msg make_end_msg(){
    int client_pid = getpid();
    char str_client_pid[MSG_SIZE];
    snprintf(str_client_pid, MSG_SIZE, "%d", client_pid);

    struct msg new_stop_msg = init_msg(END, str_client_pid);
    
    return new_stop_msg;
}


void extract_info(char *user_input, char *task_buffer, char *msg_buffer){

    if(strcmp(user_input, "") == 0) return;

    int last_index = 0;

    for(int i = 0; i < MSG_SIZE && user_input[i] != ' ' && user_input[i] != '\0'; i++){
        last_index = i;
    }

    last_index++;

    for(int i = 0; i < last_index; i++){
        task_buffer[i] = user_input[i];
    }

    int j = 0;
    for(int i = last_index; user_input[i] != '\0' && i < MSG_SIZE; i++){
        msg_buffer[j] = user_input[i];
        j++;
    }

    // if(msg_buffer[0] == ' ')
}


void remove_private_queue(int private_queue_id){
    if((msgctl(private_queue_id, IPC_RMID, NULL)) < 0){
        perror("Failed to remove the private queue!");
        exit(1);
    }
}



int main(){

    // char user_input[MSG_SIZE];
    // char task_buffer[MSG_SIZE];
    // char msg_buffer[MSG_SIZE];
    // fill_nulls(user_input);
    // fill_nulls(task_buffer);
    // fill_nulls(msg_buffer);

    // char* asd = "MIRROR asd";

    // for(int i = 0; i < 16; i++){
    //     user_input[i] = asd[i];
    // }

    // extract_info(user_input, task_buffer, msg_buffer);

    // printf("%sw\n", user_input);
    // printf("%sw\n", task_buffer);
    // printf("%sw\n", msg_buffer);



    

    key_t public_queue_key;
    int public_queue_id;
    int private_queue_id;

    //Getting public queue key
    public_queue_key = ftok(public_key_path, public_key_id);

    //Opening public message queue
    if((public_queue_id = msgget(public_queue_key, 0000)) < 0){
        perror("Failed to open a public queue!");
        exit(1);
    }

    //Creating private message queue
    if((private_queue_id = msgget(IPC_PRIVATE, IPC_CREAT | IPC_EXCL | 0666)) < 0){
        perror("Failed to create a private queue!");
        exit(1);
    }




    struct msg start_msg = make_start_msg(private_queue_id);

    struct msg recieved_msg = init_msg(0, "");

    //Sending start message
    if(msgsnd(public_queue_id, &start_msg, MSG_SIZE, 0) < 0){
        perror("Failed to send to a message queue!");
        exit(1);
    }

    //Waiting for response
    if(msgrcv(private_queue_id, &recieved_msg, MSG_SIZE, 0, 0) < 0){
        perror("Failed to recieve a message queue!");
        exit(1);
    }

    if(strcmp(recieved_msg.txt, "Hello") != 0){
        remove_private_queue(private_queue_id);
        return 1;
    }


    //Preparing user input buffers
    char user_input[MSG_SIZE];

    char task_buffer[MSG_SIZE];

    char msg_buffer[MSG_SIZE];

    struct msg new_msg;


    while(1){
        //Cleaning buffers
        fill_nulls(msg_buffer);
        fill_nulls(task_buffer);
        fill_nulls(user_input);

        //Getting user's input
        printf("Input your task: ");
        fgets(user_input, MSG_SIZE, stdin);
        remove_trailing_newline(user_input);

        extract_info(user_input, task_buffer, msg_buffer);

        if(strcmp(task_buffer, "TIME") == 0){
            new_msg = make_time_msg();
        }

        else if(strcmp(task_buffer, "MIRROR") == 0){
            new_msg = make_mirror_msg(msg_buffer);
        }

        else if(strcmp(task_buffer, "STOP") == 0){
            new_msg = make_stop_msg(msg_buffer);
        }

        else if(strcmp(task_buffer, "END") == 0){
            new_msg = make_end_msg(msg_buffer);
        }

        else{
            printf("Invalid input, try again...\n");
            continue;
        }
        
        if(msgsnd(public_queue_id, &new_msg, MSG_SIZE, 0) < 0){
            printf("Server is dead!\n");
            remove_private_queue(private_queue_id);
            exit(1);
        }

        if(msgrcv(private_queue_id, &recieved_msg, MSG_SIZE, 0, 0) < 0){
            perror("Failed to recieve a message queue!");
            exit(1);
        }

        printf("Server returns: %s\n", recieved_msg.txt);

        if(strcmp(recieved_msg.txt, "Goodbye") == 0) break;
    }



    //Removing private message queue
    remove_private_queue(private_queue_id);
    exit(0);
}