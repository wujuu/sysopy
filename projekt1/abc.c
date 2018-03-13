#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include "dblocks.h"
#include "sblocks.h"


int main(){
    int j = 0;

    struct rusage ru;
    struct timeval sys_start, sys_end, user_start, user_end;
    
    getrusage(RUSAGE_SELF, &ru);
    sys_start = ru.ru_stime;
    user_start = ru.ru_utime;

    for(int i = 0 ; i < 1000000000; i++){
        j = 1;
    }

    getrusage(RUSAGE_SELF, &ru);
    sys_end = ru.ru_stime;
    user_end = ru.ru_utime;

    printf("%ld.%06ld\n", sys_end.tv_sec, sys_end.tv_usec);
    printf("%ld.%06ld\n", user_end.tv_sec, user_end.tv_usec);

    struct rusage ru1;

    getrusage(RUSAGE_SELF, &ru1);
    sys_start = ru1.ru_stime;
    user_start = ru1.ru_utime;

    for(int i = 0 ; i < 1000000000; i++){
        j = 1;
    }

    getrusage(RUSAGE_SELF, &ru);
    sys_end = ru1.ru_stime;
    user_end = ru1.ru_utime;
    long int r = sys_end.tv_sec - sys_start.tv_sec;

    printf("%ld.%06ld\n", r, sys_end.tv_usec);
    printf("%ld.%06ld\n", user_end.tv_sec, user_end.tv_usec);
}
