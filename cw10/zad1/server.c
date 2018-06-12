#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <pthread.h>
#include <sys/select.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include "info.h"


//global clients array
struct client *clients_array[MAX_CLIENTS];

int public_socket_fd = -1;
int highest_fd = 0;

// struct task tasks[MAX_TASKS];

//CLIENT FUNCTIONS
struct client *init_client(char *client_name){
    if(strlen(client_name) < MAX_CLIENT_NAME_LENGTH){
        struct client *new_client = malloc(sizeof(struct client));
        strcpy(new_client -> name, client_name);
        new_client -> socket_fd = -1;
        return new_client;
    }

    return NULL;    
}

int is_full(){
    for(int i = 0; i < MAX_CLIENTS; i++)
        if(clients_array[i] == NULL)
            return 0;

    return 1;
}

int find_free_index(){
    if(!is_full())
        for(int i = 0; i < MAX_CLIENTS; i++)
            if(clients_array[i] == NULL)
                return i;
    
    return -1;
}

int insert_client(struct client *new_client){
    int free_index = find_free_index();

    if(free_index != -1){
        clients_array[free_index] = new_client;
        return 0;
    }

    return -1;
}

struct client *get_client_by_fd(int fd){
    for(int i = 0; i < MAX_CLIENTS; i++)
        if(clients_array[i] != NULL && clients_array[i] -> socket_fd == fd)
            return clients_array[i];
    
    return NULL;
}

int delete_client_by_fd(int fd){
    for(int i = 0; i < MAX_CLIENTS; i++)
        if(clients_array[i] != NULL && clients_array[i] -> socket_fd == fd){
            free(clients_array[i]);
            clients_array[i] = NULL;
            return 0;
        }
    
    return -1;
}

struct client *get_random_client(){
    int active_clients = 0;

    for(int i = 0; i < MAX_CLIENTS; i++)
        if(clients_array[i] != NULL)
            active_clients++;

    int rand_client_num = rand() % active_clients + 1;

    int rand_client_counter = 0;

    for(int i = 0; i < MAX_CLIENTS; i++){
        if(clients_array[i] != NULL){
            rand_client_counter++;
            if(rand_client_counter == rand_client_num)
                return clients_array[i];
        }
            
    }

    return NULL;
}

void print_client(struct client *client){
    if(client != NULL)
        printf("name: %s", client -> name);
    else
        printf("empty");
    printf("\n");
}

void print_clients(){
    for(int i = 0; i < MAX_CLIENTS; i++){
        printf("%i: ", i);
        print_client(clients_array[i]);
    }
    printf("\n");
}

//TASK FUNCTIONS

//PROPER TASK FORMAT(SINGLE SPACES):ADD 12333 222
void *interpreter(void *arg){
    struct task *current_task = NULL;

    char *user_input = NULL;
    size_t size = 0;

    do {
        printf("> ");
        getline(&user_input, &size, stdin);

        current_task = parse_task(user_input);

        if(current_task != NULL){
            char task_buff[128];

            task_to_string(current_task, task_buff);

            struct client *rand_client = get_random_client();

            if(rand_client == NULL){
                printf("ni mo komu\n");
                continue;
            }

            int client_socket_fd = rand_client -> socket_fd;

            write(client_socket_fd, task_buff, 128);
        }
        else
            printf("Error: invalid task name...\n");
        
    } while(1);

    return NULL;
}


void *recieve_responses(void *arg){
    fd_set set, read_set;
    FD_ZERO(&set);
    FD_SET(public_socket_fd, &set);

    char buff[128];
    ssize_t nread;

    while(1){
        read_set = set;

        select(highest_fd + 1, &read_set, NULL, NULL, NULL);

        for(int fd = 0; fd <= highest_fd; fd++){
            if(FD_ISSET(fd, &read_set)){

                if(fd == public_socket_fd){
                    int client_socket_fd = accept(public_socket_fd, NULL, 0);
                    if(client_socket_fd > highest_fd)
                        highest_fd = client_socket_fd;

                    FD_SET(client_socket_fd, &set);

                    struct client *new_client = init_client("asd");
                    new_client -> socket_fd = client_socket_fd;

                    insert_client(new_client);

                    printf("Client number: %i is ready to work!\n", client_socket_fd);

                }
                else {
                    nread = read(fd, buff, sizeof(buff));
                    
                    if(nread == 0) {
                        FD_CLR(fd, &set);
                        delete_client_by_fd(fd);

                        if(fd == highest_fd) highest_fd--;

                        close(fd);
                    } else {
                        printf("I got answer from %i: %s\n", fd, buff);
                        write(fd, "I got your message!\n", sizeof("I got your message!"));
                    }
                }
            }
        }
    }

    return NULL;


}


int main(){
    
    srand(time(NULL));

    int max_listen_queue = 10;

    // INITIALIZING CLIENTS ARRAY
    for(int i = 0; i < MAX_CLIENTS; i++) clients_array[i] = NULL;
    

    struct sockaddr_un sa;

    strcpy(sa.sun_path, "public_server_socket");
    sa.sun_family = AF_UNIX;

    public_socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(public_socket_fd > highest_fd)
        highest_fd = public_socket_fd;

    bind(public_socket_fd, (struct sockaddr *) &sa, sizeof(sa));

    listen(public_socket_fd, max_listen_queue);


    pthread_t intepreter_thread, responses_thread;

    pthread_create(&intepreter_thread, NULL, interpreter, NULL);
    pthread_create(&responses_thread, NULL, recieve_responses, NULL);

    pthread_join(intepreter_thread, NULL);
    pthread_join(intepreter_thread, NULL);


    close(public_socket_fd);


    return 0;
}