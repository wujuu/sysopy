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

int main(){

    char test[] = "AdDD 12 34";

    char task_buff[128];

    struct task *curr_task = parse_task(test);

    if(curr_task == NULL) exit(1);

    task_to_string(curr_task, task_buff);

    printf("%s\n", task_buff);

    curr_task = parse_task(task_buff);

    if(curr_task == NULL) printf("ja pierdole\n");


    exit(0);
}