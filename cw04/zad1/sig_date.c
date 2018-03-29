#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>







int main(){

    time_t raw_now;
    struct tm *nice_now;

    while(1){
        
        time(&raw_now);
        nice_now = localtime(&raw_now);

        printf("%s \n", asctime(nice_now));

        sleep(1);
    }

}