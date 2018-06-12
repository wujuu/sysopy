#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

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
    int queue_id = msgget(queue_key, IPC_CREAT | S_IRWXU);




    char txt[10];
    scanf("%s", txt);
    

    struct msg recieved_msg;

    if(msgrcv(queue_id, &recieved_msg, MSG_SIZE, 0, 0) == -1){
        perror("cant recieve");
    }

    printf("%s\n", recieved_msg.content);

    msgctl(queue_id, IPC_RMID, NULL);


    exit(0);
}