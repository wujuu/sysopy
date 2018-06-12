#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

#define MSG_SIZE 128

struct msg{
    long type;
    char content[MSG_SIZE];
};



int main() {

    const char pathname[] = "/home/patryk/Desktop";
    int proj_id = 100;
    
    //intializing
    key_t queue_key = ftok(pathname, proj_id);
    int queue_id = msgget(queue_key, 0000);



    struct msg message_to_send;

    message_to_send.type = 1;
    strcpy(message_to_send.content, "no siemanko kurwa");

    if(msgsnd(queue_id, &message_to_send, MSG_SIZE, 0) == -1){
        perror("Cant send");
        exit(1);
    }

    exit(0);
}