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


        printf("DYNAMIC \n");
        struct rusage ru;
        struct timeval s_star, s_end, u_star, u_end;
        
        getrusage(RUSAGE_SELF, &ru);
        s_star = ru.ru_stime;
        u_star = ru.ru_utime;
        long int r_star_sec = s_star.tv_sec + u_star.tv_sec;
        long int r_star_usec = s_star.tv_usec + u_star.tv_usec;

        charBlockArray* dynamicArray = initCharBlockArray(blocksSize, charsSize);

        getrusage(RUSAGE_SELF, &ru);
        s_end = ru.ru_stime;
        u_end = ru.ru_utime;
        long int r_end_sec = s_end.tv_sec + u_end.tv_sec;
        long int r_end_usec = s_end.tv_usec + u_end.tv_usec;

        printf("sys: %ld.%06ld, usr: %ld.%06ld, real: %ld.%06ld\n", s_end.tv_sec - s_star.tv_sec, s_end.tv_usec - s_star.tv_usec, u_end.tv_sec - u_star.tv_sec, u_end.tv_usec - u_star.tv_usec, r_end_sec - r_star_sec, r_end_usec - r_star_usec);



        //NOT MEASURING for testing's sake;
        dynamicArray = makeRandCharBlockArray(blocksSize, charsSize);


        struct rusage ru1;

        getrusage(RUSAGE_SELF, &ru1);
        s_star = ru1.ru_stime;
        u_star = ru1.ru_utime;
        r_star_sec = s_star.tv_sec + u_star.tv_sec;
        r_star_usec = s_star.tv_usec + u_star.tv_usec;

        getMinBlock(dynamicArray, closestTo);

        getrusage(RUSAGE_SELF, &ru1);
        s_end = ru1.ru_stime;
        u_end = ru1.ru_utime;
        r_end_sec = s_end.tv_sec + u_end.tv_sec;
        r_end_usec = s_end.tv_usec + u_end.tv_usec;

        printf("sys: %ld.%06ld, usr: %ld.%06ld, real: %ld.%06ld\n", s_end.tv_sec - s_star.tv_sec, s_end.tv_usec - s_star.tv_usec, u_end.tv_sec - u_star.tv_sec, u_end.tv_usec - u_star.tv_usec, r_end_sec - r_star_sec, r_end_usec - r_star_usec);

        int times = 0;    
        int i = 0; 
        struct rusage ru2;

        getrusage(RUSAGE_SELF, &ru2);
        s_star = ru2.ru_stime;
        u_star = ru2.ru_utime;
        r_star_sec = s_star.tv_sec + u_star.tv_sec;
        r_star_usec = s_star.tv_usec + u_star.tv_usec;

        
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
        s_end = ru2.ru_stime;
        u_end = ru2.ru_utime;
        r_end_sec = s_end.tv_sec + u_end.tv_sec;
        r_end_usec = s_end.tv_usec + u_end.tv_usec;

        printf("sys: %ld.%06ld, usr: %ld.%06ld, real: %ld.%06ld\n", s_end.tv_sec - s_star.tv_sec, s_end.tv_usec - s_star.tv_usec, u_end.tv_sec - u_star.tv_sec, u_end.tv_usec - u_star.tv_usec, r_end_sec - r_star_sec, r_end_usec - r_star_usec);
        



        times = 0;
        i = 0;
        struct rusage ru3;


        getrusage(RUSAGE_SELF, &ru3);
        s_star = ru3.ru_stime;
        u_star = ru3.ru_utime;
        r_star_sec = s_star.tv_sec + u_star.tv_sec;
        r_star_usec = s_star.tv_usec + u_star.tv_usec;

        while(times < times_add_del_mix){
            delBlock(dynamicArray, i);
            addBlock(dynamicArray, i, charsSize, tmp);

            i = (i+1) % (blocksSize);

            times++;
        }

        getrusage(RUSAGE_SELF, &ru3);
        s_end = ru3.ru_stime;
        u_end = ru3.ru_utime;
        r_end_sec = s_end.tv_sec + u_end.tv_sec;
        r_end_usec = s_end.tv_usec + u_end.tv_usec;

        printf("sys: %ld.%06ld, usr: %ld.%06ld, real: %ld.%06ld\n", s_end.tv_sec - s_star.tv_sec, s_end.tv_usec - s_star.tv_usec, u_end.tv_sec - u_star.tv_sec, u_end.tv_usec - u_star.tv_usec, r_end_sec - r_star_sec, r_end_usec - r_star_usec);

        printf("STATIC\n");
        
        getrusage(RUSAGE_SELF, &ru);
        s_star = ru.ru_stime;
        u_star = ru.ru_utime;
        r_star_sec = s_star.tv_sec + u_star.tv_sec;
        r_star_usec = s_star.tv_usec + u_star.tv_usec;

        staticCharBlockArray staticArray = staticInitCharBlockArray(blocksSize, charsSize);

        getrusage(RUSAGE_SELF, &ru);
        s_end = ru.ru_stime;
        u_end = ru.ru_utime;
        r_end_sec = s_end.tv_sec + u_end.tv_sec;
        r_end_usec = s_end.tv_usec + u_end.tv_usec;

        printf("sys: %ld.%06ld, usr: %ld.%06ld, real: %ld.%06ld\n", s_end.tv_sec - s_star.tv_sec, s_end.tv_usec - s_star.tv_usec, u_end.tv_sec - u_star.tv_sec, u_end.tv_usec - u_star.tv_usec, r_end_sec - r_star_sec, r_end_usec - r_star_usec);





        //for testing's sake;
        staticArray = staticMakeRandCharBlockArray(blocksSize, charsSize);



        getrusage(RUSAGE_SELF, &ru1);
        s_star = ru1.ru_stime;
        u_star = ru1.ru_utime;
        r_star_sec = s_star.tv_sec + u_star.tv_sec;
        r_star_usec = s_star.tv_usec + u_star.tv_usec;

        staticGetMinIndex(staticArray, closestTo);

        getrusage(RUSAGE_SELF, &ru1);
        s_end = ru1.ru_stime;
        u_end = ru1.ru_utime;
        r_end_sec = s_end.tv_sec + u_end.tv_sec;
        r_end_usec = s_end.tv_usec + u_end.tv_usec;

        printf("sys: %ld.%06ld, usr: %ld.%06ld, real: %ld.%06ld\n", s_end.tv_sec - s_star.tv_sec, s_end.tv_usec - s_star.tv_usec, u_end.tv_sec - u_star.tv_sec, u_end.tv_usec - u_star.tv_usec, r_end_sec - r_star_sec, r_end_usec - r_star_usec);




        i = 0;
        times = 0;    



        getrusage(RUSAGE_SELF, &ru2);
        s_star = ru2.ru_stime;
        u_star = ru2.ru_utime;
        r_star_sec = s_star.tv_sec + u_star.tv_sec;
        r_star_usec = s_star.tv_usec + u_star.tv_usec;


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
        s_end = ru2.ru_stime;
        u_end = ru2.ru_utime;
        r_end_sec = s_end.tv_sec + u_end.tv_sec;
        r_end_usec = s_end.tv_usec + u_end.tv_usec;
        
        printf("sys: %ld.%06ld, usr: %ld.%06ld, real: %ld.%06ld\n", s_end.tv_sec - s_star.tv_sec, s_end.tv_usec - s_star.tv_usec, u_end.tv_sec - u_star.tv_sec, u_end.tv_usec - u_star.tv_usec, r_end_sec - r_star_sec, r_end_usec - r_star_usec);




        i = 0;
        times = 0;



        getrusage(RUSAGE_SELF, &ru3);
        s_star = ru3.ru_stime;
        u_star = ru3.ru_utime;
        r_star_sec = s_star.tv_sec + u_star.tv_sec;
        r_star_usec = s_star.tv_usec + u_star.tv_usec;

        while(times <= times_add_del_mix){
            staticDelBlock(&staticArray, i);
            staticAddBlock(&staticArray, i, charsSize, tmp);
            i = (i+1);
            i = i % (blocksSize);
            times++;
        }

        getrusage(RUSAGE_SELF, &ru3);
        s_end = ru3.ru_stime;
        u_end = ru3.ru_utime;
        r_end_sec = s_end.tv_sec + u_end.tv_sec;
        r_end_usec = s_end.tv_usec + u_end.tv_usec;

        printf("sys: %ld.%06ld, usr: %ld.%06ld, real: %ld.%06ld\n", s_end.tv_sec - s_star.tv_sec, s_end.tv_usec - s_star.tv_usec, u_end.tv_sec - u_star.tv_sec, u_end.tv_usec - u_star.tv_usec, r_end_sec - r_star_sec, r_end_usec - r_star_usec);

        


    



        

    return 0;
}