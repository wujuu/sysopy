#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "client_server.h"


struct txt_msg init_txt_msg(long new_type, char* new_txt){
    struct txt_msg new_txt_msg;

    new_txt_msg.type = new_type;

    for(int i = 0; i < TXT_MSG_SIZE; i++)
        new_txt_msg.txt[i] = '\0';

    for(int i = 0; i < TXT_MSG_SIZE && new_txt[i] != '\0'; i++)
        new_txt_msg.txt[i] = new_txt[i];

    return new_txt_msg;
}


void print_queue_stats(int queue_id){
    struct msqid_ds queue_stat;

    if((msgctl(queue_id, IPC_STAT, &queue_stat)) < 0){
        perror("Failed to get queue stats!");
        exit(1);
    }

    printf("#messages = %li, max #bytes = %li\n", queue_stat.msg_qnum, queue_stat.msg_qbytes);
    printf("pid of last sender = %i, pid of last reciever = %i\n", queue_stat.msg_lspid, queue_stat.msg_lrpid);
    printf("time of last send = %li, time of last recieve = %li\n", queue_stat.msg_stime, queue_stat.msg_rtime);
}