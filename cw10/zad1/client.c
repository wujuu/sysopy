#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "info.h"


int get_result(struct task *input_task){
    if(strcmp(input_task -> task_name, "ADD") == 0) 
        return input_task -> x + input_task -> y;

    if(strcmp(input_task -> task_name, "SUB") == 0) 
        return input_task -> x - input_task -> y;

    if(strcmp(input_task -> task_name, "MUL") == 0) 
        return input_task -> x * input_task -> y;

    if(strcmp(input_task -> task_name, "DIV") == 0) 
        return input_task -> x / input_task -> y;

    return -1;
}

int main(){

    int server_socket_fd;
    

    struct sockaddr_un sa;

    strcpy(sa.sun_path, "public_server_socket");
    sa.sun_family = AF_UNIX;

    server_socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);

    //START HERE
    connect(server_socket_fd, (struct sockaddr *) &sa, sizeof(sa));

    while(1){
        char buff[128];
        char res_buff[128];
        read(server_socket_fd, buff, sizeof(buff));

        struct task *current_task = parse_task(buff);

        if(current_task == NULL){
            printf("somethign is no yes\n");
            continue;
        }

        int result = get_result(current_task);

        char str[10];
        sprintf(str, "%d", result);

        strcpy(res_buff, "The result is: ");
        strcpy(res_buff, str);


        write(server_socket_fd, res_buff, 128);

        read(server_socket_fd, buff, sizeof(buff));
        printf("Server: %s\n", buff);
    }

    close(server_socket_fd);

    return 0;
}