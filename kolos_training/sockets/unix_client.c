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
    struct sockaddr_un sa;

    int socket_fd;
    char buff[128];

    sa.sun_family = AF_UNIX;
    strcpy(sa.sun_path, "some_socket");

    socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);

    //START HERE
    connect(socket_fd, (struct sockaddr *) &sa, sizeof(sa));

    write(socket_fd, "Hello father", sizeof("Hello from client"));
    read(socket_fd, buff, sizeof(buff));

    printf("I got '%s' from server\n", buff);

    close(socket_fd);

    return 0;
}