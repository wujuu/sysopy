#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <netinet/in.h>
#include <fcntl.h>

int main(){
    struct sockaddr_un sa;

    int socket_fd, client_socket_fd;
    char buff[128];

    sa.sun_family = AF_UNIX;
    strcpy(sa.sun_path, "some_socket");

    socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);


    bind(socket_fd, (struct sockaddr *) &sa, sizeof(sa));

    listen(socket_fd, 16);

    //START HERE, BITCH KURWA

    client_socket_fd = accept(socket_fd, NULL, 0);

    read(client_socket_fd, buff, sizeof(buff));
    printf("I got '%s' from client\n", buff);

    write(client_socket_fd, "Go home biatch", sizeof("Go home biatch"));
    
    close(client_socket_fd);
    close(socket_fd);

    


    return 0;
}