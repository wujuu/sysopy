#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>



int main() {

    struct mq_attr new_queue_attrs;

    // new_queue_attrs.mq_flags = 0;
    new_queue_attrs.mq_maxmsg = 10;
    new_queue_attrs.mq_msgsize = 128;
    // new_queue_attrs.mq_curmsgs = -1;
    

    mqd_t new_queue_des = -1;

    const char queue_name[] = "/home2";

    if((new_queue_des = mq_open(queue_name, O_CREAT | O_RDWR, S_IRWXU, &new_queue_attrs)) == -1){
        perror("Failed to create a new queue");
        exit(1);
    }


    char txt[10];
    scanf("%s", txt);

    char buff[128];

    if(mq_receive(new_queue_des, buff, 128, NULL) == -1){
        perror("Couldn't recieve");
    }

    printf("%s\n", buff);


    if(mq_close(new_queue_des) == -1){
        perror("Can't close this queue");
        exit(1);
    }

    // if(mq_unlink(queue_name) == -1){
    //     perror("Can't delete this queue");
    //     exit(1);
    // }

    exit(0);
}