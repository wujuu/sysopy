#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>

int main(){
    // INITIALIZING CLIENTS ARRAY
    // struct client *clients[MAX_CLIENTS];
    // for(int i = 0; i < MAX_CLIENTS; i++) clients[i] = NULL;

    // interpreter();


    struct sockaddr_in sa;

    int socket_fd;
    char buff[128];

    sa.sin_family = AF_INET;
    sa.sin_port = htons(50000);
    sa.sin_addr.s_addr = INADDR_LOOPBACK;


    socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    //START HERE
    connect(socket_fd, (struct sockaddr *) &sa, sizeof(sa));

    write(socket_fd, "Hello father", sizeof("Hello from client"));
    read(socket_fd, buff, sizeof(buff));

    printf("I got '%s' from server\n", buff);

    close(socket_fd);

    return 0;
}