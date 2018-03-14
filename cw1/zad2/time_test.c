#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include "dblocks.h"
#include "sblocks.h"






int main(int argc, char** argv){
    //1000 1000 20000 1000 100000;

    int blocksSize = atoi(argv[1]);
    int charsSize = atoi(argv[2]);
    int closestTo = atoi(argv[3]);
    int times_add_del_cont = atoi(argv[4]);
    int times_add_del_mix = atoi(argv[5]);


    char* tmp = calloc(charsSize, sizeof(char));

    for(int j = 0; j < charsSize; j++){
        tmp[j] = 32 + rand() % 95;
    }

     
    const char* dynamic = "dynamic";


        printf("DYNAMIC\n");
        struct rusage ru;
        struct timeval sys_start, sys_end, user_start, user_end;
        
        getrusage(RUSAGE_SELF, &ru);
        sys_start = ru.ru_stime;
        user_start = ru.ru_utime;
        long int real_time_start_sec = sys_start.tv_sec + user_start.tv_sec;
        long int real_time_start_usec = sys_start.tv_usec + user_start.tv_usec;

        charBlockArray* dynamicArray = initCharBlockArray(blocksSize, charsSize);

        getrusage(RUSAGE_SELF, &ru);
        sys_end = ru.ru_stime;
        user_end = ru.ru_utime;
        long int real_time_end_sec = sys_end.tv_sec + user_end.tv_sec;
        long int real_time_end_usec = sys_end.tv_usec + user_end.tv_usec;

        printf("sys: %ld.%06ld, usr: %ld.%06ld, real: %ld.%06ld\n", sys_end.tv_sec - sys_start.tv_sec, sys_end.tv_usec - sys_start.tv_usec, user_end.tv_sec - user_start.tv_sec, user_end.tv_usec - user_start.tv_usec, real_time_end_sec - real_time_start_sec, real_time_end_usec - real_time_start_usec);



        //NOT MEASURING for testing's sake;
        dynamicArray = makeRandCharBlockArray(blocksSize, charsSize);


        struct rusage ru1;

        getrusage(RUSAGE_SELF, &ru1);
        sys_start = ru1.ru_stime;
        user_start = ru1.ru_utime;
        real_time_start_sec = sys_start.tv_sec + user_start.tv_sec;
        real_time_start_usec = sys_start.tv_usec + user_start.tv_usec;

        getMinBlock(dynamicArray, closestTo);

        getrusage(RUSAGE_SELF, &ru1);
        sys_end = ru1.ru_stime;
        user_end = ru1.ru_utime;
        real_time_end_sec = sys_end.tv_sec + user_end.tv_sec;
        real_time_end_usec = sys_end.tv_usec + user_end.tv_usec;

        printf("sys: %ld.%06ld, usr: %ld.%06ld, real: %ld.%06ld\n", sys_end.tv_sec - sys_start.tv_sec, sys_end.tv_usec - sys_start.tv_usec, user_end.tv_sec - user_start.tv_sec, user_end.tv_usec - user_start.tv_usec, real_time_end_sec - real_time_start_sec, real_time_end_usec - real_time_start_usec);

        int times = 0;    
        int i = 0; 
        struct rusage ru2;

        getrusage(RUSAGE_SELF, &ru2);
        sys_start = ru2.ru_stime;
        user_start = ru2.ru_utime;
        real_time_start_sec = sys_start.tv_sec + user_start.tv_sec;
        real_time_start_usec = sys_start.tv_usec + user_start.tv_usec;

        
        while(times < times_add_del_cont){
            delBlock(dynamicArray, i);
            i = (i+1) % (blocksSize);
            times++;
        }

        times = 0;
        i = 0;

        while(times < times_add_del_cont){
            addBlock(dynamicArray, i, charsSize, tmp);
            i = (i+1) % (blocksSize);
            times++;
        }

        getrusage(RUSAGE_SELF, &ru2);
        sys_end = ru2.ru_stime;
        user_end = ru2.ru_utime;
        real_time_end_sec = sys_end.tv_sec + user_end.tv_sec;
        real_time_end_usec = sys_end.tv_usec + user_end.tv_usec;

        printf("sys: %ld.%06ld, usr: %ld.%06ld, real: %ld.%06ld\n", sys_end.tv_sec - sys_start.tv_sec, sys_end.tv_usec - sys_start.tv_usec, user_end.tv_sec - user_start.tv_sec, user_end.tv_usec - user_start.tv_usec, real_time_end_sec - real_time_start_sec, real_time_end_usec - real_time_start_usec);
        



        times = 0;
        i = 0;
        struct rusage ru3;


        getrusage(RUSAGE_SELF, &ru3);
        sys_start = ru3.ru_stime;
        user_start = ru3.ru_utime;
        real_time_start_sec = sys_start.tv_sec + user_start.tv_sec;
        real_time_start_usec = sys_start.tv_usec + user_start.tv_usec;

        while(times < times_add_del_mix){
            delBlock(dynamicArray, i);
            addBlock(dynamicArray, i, charsSize, tmp);

            i = (i+1) % (blocksSize);

            times++;
        }

        getrusage(RUSAGE_SELF, &ru3);
        sys_end = ru3.ru_stime;
        user_end = ru3.ru_utime;
        real_time_end_sec = sys_end.tv_sec + user_end.tv_sec;
        real_time_end_usec = sys_end.tv_usec + user_end.tv_usec;

        printf("sys: %ld.%06ld, usr: %ld.%06ld, real: %ld.%06ld\n", sys_end.tv_sec - sys_start.tv_sec, sys_end.tv_usec - sys_start.tv_usec, user_end.tv_sec - user_start.tv_sec, user_end.tv_usec - user_start.tv_usec, real_time_end_sec - real_time_start_sec, real_time_end_usec - real_time_start_usec);

        printf("STATIC\n");
        
        getrusage(RUSAGE_SELF, &ru);
        sys_start = ru.ru_stime;
        user_start = ru.ru_utime;
        real_time_start_sec = sys_start.tv_sec + user_start.tv_sec;
        real_time_start_usec = sys_start.tv_usec + user_start.tv_usec;

        staticCharBlockArray staticArray = staticInitCharBlockArray(blocksSize, charsSize);

        getrusage(RUSAGE_SELF, &ru);
        sys_end = ru.ru_stime;
        user_end = ru.ru_utime;
        real_time_end_sec = sys_end.tv_sec + user_end.tv_sec;
        real_time_end_usec = sys_end.tv_usec + user_end.tv_usec;

        printf("sys: %ld.%06ld, usr: %ld.%06ld, real: %ld.%06ld\n", sys_end.tv_sec - sys_start.tv_sec, sys_end.tv_usec - sys_start.tv_usec, user_end.tv_sec - user_start.tv_sec, user_end.tv_usec - user_start.tv_usec, real_time_end_sec - real_time_start_sec, real_time_end_usec - real_time_start_usec);





        //for testing's sake;
        staticArray = staticMakeRandCharBlockArray(blocksSize, charsSize);



        getrusage(RUSAGE_SELF, &ru1);
        sys_start = ru1.ru_stime;
        user_start = ru1.ru_utime;
        real_time_start_sec = sys_start.tv_sec + user_start.tv_sec;
        real_time_start_usec = sys_start.tv_usec + user_start.tv_usec;

        staticGetMinIndex(staticArray, closestTo);

        getrusage(RUSAGE_SELF, &ru1);
        sys_end = ru1.ru_stime;
        user_end = ru1.ru_utime;
        real_time_end_sec = sys_end.tv_sec + user_end.tv_sec;
        real_time_end_usec = sys_end.tv_usec + user_end.tv_usec;

        printf("sys: %ld.%06ld, usr: %ld.%06ld, real: %ld.%06ld\n", sys_end.tv_sec - sys_start.tv_sec, sys_end.tv_usec - sys_start.tv_usec, user_end.tv_sec - user_start.tv_sec, user_end.tv_usec - user_start.tv_usec, real_time_end_sec - real_time_start_sec, real_time_end_usec - real_time_start_usec);




        i = 0;
        times = 0;    



        getrusage(RUSAGE_SELF, &ru2);
        sys_start = ru2.ru_stime;
        user_start = ru2.ru_utime;
        real_time_start_sec = sys_start.tv_sec + user_start.tv_sec;
        real_time_start_usec = sys_start.tv_usec + user_start.tv_usec;


        while(times < times_add_del_cont){
            staticDelBlock(&staticArray, i);
            i = (i+1) % blocksSize;
            times++;
        }


        i = 0;
        times = 0;

        while(times <= times_add_del_cont){
            staticAddBlock(&staticArray, i, charsSize, tmp);
            i = (i+1);
            i = i % (blocksSize);
            times++;
        }

        getrusage(RUSAGE_SELF, &ru2);
        sys_end = ru2.ru_stime;
        user_end = ru2.ru_utime;
        real_time_end_sec = sys_end.tv_sec + user_end.tv_sec;
        real_time_end_usec = sys_end.tv_usec + user_end.tv_usec;
        
        printf("sys: %ld.%06ld, usr: %ld.%06ld, real: %ld.%06ld\n", sys_end.tv_sec - sys_start.tv_sec, sys_end.tv_usec - sys_start.tv_usec, user_end.tv_sec - user_start.tv_sec, user_end.tv_usec - user_start.tv_usec, real_time_end_sec - real_time_start_sec, real_time_end_usec - real_time_start_usec);




        i = 0;
        times = 0;



        getrusage(RUSAGE_SELF, &ru3);
        sys_start = ru3.ru_stime;
        user_start = ru3.ru_utime;
        real_time_start_sec = sys_start.tv_sec + user_start.tv_sec;
        real_time_start_usec = sys_start.tv_usec + user_start.tv_usec;

        while(times <= times_add_del_mix){
            staticDelBlock(&staticArray, i);
            staticAddBlock(&staticArray, i, charsSize, tmp);
            i = (i+1);
            i = i % (blocksSize);
            times++;
        }

        getrusage(RUSAGE_SELF, &ru3);
        sys_end = ru3.ru_stime;
        user_end = ru3.ru_utime;
        real_time_end_sec = sys_end.tv_sec + user_end.tv_sec;
        real_time_end_usec = sys_end.tv_usec + user_end.tv_usec;

        printf("sys: %ld.%06ld, usr: %ld.%06ld, real: %ld.%06ld\n", sys_end.tv_sec - sys_start.tv_sec, sys_end.tv_usec - sys_start.tv_usec, user_end.tv_sec - user_start.tv_sec, user_end.tv_usec - user_start.tv_usec, real_time_end_sec - real_time_start_sec, real_time_end_usec - real_time_start_usec);

        


    



        

    return 0;
}