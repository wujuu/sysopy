#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/socket.h>
#include <sys/un.h>


#define MAX_CLIENTS 4
#define MAX_CLIENT_NAME_LENGTH 8

#define MAX_TASK_NAME_LENGTH 16

struct client{
    char name[MAX_CLIENT_NAME_LENGTH];
    int socket_fd;
};

struct task{
    char task_name[MAX_TASK_NAME_LENGTH];
    int x, y;
};


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

int is_full(struct client *clients_array[]){
    for(int i = 0; i < MAX_CLIENTS; i++)
        if(clients_array[i] == NULL)
            return 0;

    return 1;
}

int find_free_index(struct client *clients_array[]){
    if(!is_full(clients_array))
        for(int i = 0; i < MAX_CLIENTS; i++)
            if(clients_array[i] == NULL)
                return i;
    
    return -1;
}

int insert_client(struct client *clients_array[], struct client *new_client){
    int free_index = find_free_index(clients_array);

    if(free_index != -1){
        clients_array[free_index] = new_client;
        return 0;
    }

    return -1;
}

struct client *get_client_by_name(struct client *clients_array[], char *name){
    for(int i = 0; i < MAX_CLIENTS; i++)
        if(clients_array[i] != NULL && (strcmp(name, clients_array[i] -> name)) == 0)
            return clients_array[i];
    
    return NULL;
}

int delete_client_by_name(struct client *clients_array[], char *name){
    for(int i = 0; i < MAX_CLIENTS; i++)
        if(clients_array[i] != NULL && (strcmp(name, clients_array[i] -> name)) == 0){
            free(clients_array[i]);
            clients_array[i] = NULL;
            return 0;
        }
    
    return -1;
}

void print_client(struct client *client){
    if(client != NULL)
        printf("name: %s", client -> name);
    else
        printf("empty");
    printf("\n");
}

void print_clients(struct client *clients_array[]){
    for(int i = 0; i < MAX_CLIENTS; i++){
        printf("%i: ", i);
        print_client(clients_array[i]);
    }
    printf("\n");
}

//TASK FUNCTIONS
char *to_upper_case(char* string){
    char* upper_string = malloc(strlen(string));
    strcpy(upper_string, string);

    for(int i = 0; i < strlen(string); i++) upper_string[i] = toupper(string[i]);

    return upper_string;
}

int is_valid_num(char* string){
    for(int i = 0; i < strlen(string); i++) 
        if ((string[i] < '0' || string[i] > '9') && (string[i] != '\n'))
            return 0;

    return 1;
}

struct task *parse_task(char* input){
    char* upper_input = to_upper_case(input);
    char *tmp = NULL;

    struct task *new_task = malloc(sizeof(struct task));


    //TASK STRING       
    tmp = strsep(&upper_input, " ");
    if(strcmp(tmp, "ADD") == 0 || strcmp(tmp, "SUB") == 0 || strcmp(tmp, "MUL") == 0 || strcmp(tmp, "DIV") == 0)
        strcpy(new_task -> task_name, tmp);
    else
        return NULL;
    
    //X OPERAND
    tmp = strsep(&upper_input, " ");
    if(is_valid_num(tmp))
        new_task -> x = atoi(tmp);
    else
        return NULL;

    //Y OPERAND
    tmp = strsep(&upper_input, " ");
    if(is_valid_num(tmp))
        new_task -> y = atoi(tmp);
    else
        return NULL;

    
    return new_task;
}

void print_task(struct task *input_task){
    if(input_task != NULL)
        printf("task: %s, x: %i, y: %i\n", input_task -> task_name, input_task -> x, input_task -> y);
}

//PROPER TASK FORMAT(SINGLE SPACES):ADD 12333 222
void interpreter(){
    struct task *current_task = NULL;

    char *user_input = NULL;
    size_t size = 0;

    do {
        printf("> ");
        getline(&user_input, &size, stdin);

        current_task = parse_task(user_input);

        if(current_task != NULL)
            print_task(current_task);
        
    } while(strcmp(user_input, "exit\n") != 0);
}


int main(){
    // INITIALIZING CLIENTS ARRAY
    // struct client *clients[MAX_CLIENTS];
    // for(int i = 0; i < MAX_CLIENTS; i++) clients[i] = NULL;

    // interpreter();


    struct sockaddr_un sa;

    strcpy(sa.sun_path, "MySocket");
    sa.sun_family = AF_UNIX;


    char txt[10];
    scanf("%s", txt);

    


    return 0;
}