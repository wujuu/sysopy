#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>




int main() {

    struct mq_attr queue_attrs;
    queue_attrs.mq_flags = -1;
    queue_attrs.mq_maxmsg = -1;
    queue_attrs.mq_msgsize = -1;
    queue_attrs.mq_curmsgs = -1;
    

    mqd_t queue_des;

    if((queue_des = mq_open("/home2", O_RDWR)) == -1){
        perror("Failed to open this queue");
        exit(1);
    }

    // if(mq_getattr(queue_des, &queue_attrs) == -1){
    //     perror("Couldn't read attrs of this queue");
    //     if(mq_close(queue_des) == -1){
    //     perror("Can't close this queue");
    //     exit(1);
    // }
    // }

    // printf("flags: %li \n", queue_attrs.mq_flags);
    // printf("maxmsg: %li \n", queue_attrs.mq_maxmsg);
    // printf("msgsize: %li \n", queue_attrs.mq_msgsize);
    // printf("curmsgs: %li\n", queue_attrs.mq_curmsgs);

    

    if(mq_send(queue_des, "Hello kurwa", sizeof("Hello kurwa"), 20) == -1){
        perror("Coulnd't send the message");
    }



    if(mq_close(queue_des) == -1){
        perror("Can't close this queue");
        exit(1);
    }

    exit(0);
}