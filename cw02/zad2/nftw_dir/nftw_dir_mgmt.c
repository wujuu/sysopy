#define _XOPEN_SOURCE 500
#include<stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <ftw.h>


int comp_mode;
time_t comp_date;

void err_sys(const char *x) 
{ 
    perror(x); 
    exit(1); 
}


void display_file_stats(const char *abs_path, const struct stat *file_stat){
    if((comp_mode == -1 && comp_date < file_stat -> st_mtime) ||
       (comp_mode == 0 && comp_date == file_stat -> st_mtime) ||
       (comp_mode == 1 && comp_date > file_stat -> st_mtime)){

        printf("Absolute path: %s\n", abs_path);
        printf("File size in bytes: %li \n", file_stat -> st_size);
        printf("File permissions: ");

        if((file_stat -> st_mode & S_IRUSR) > 0) printf("r");
        else printf("-");

        if((file_stat -> st_mode & S_IWUSR) > 0) printf("w");
        else printf("-");

        if((file_stat -> st_mode & S_IXUSR) > 0) printf("x");
        else printf("-");

        if((file_stat -> st_mode & S_IRGRP) > 0) printf("r");
        else printf("-");

        if((file_stat -> st_mode & S_IWGRP) > 0) printf("w");
        else printf("-");

        if((file_stat -> st_mode & S_IXGRP) > 0) printf("x");
        else printf("-");

        if((file_stat -> st_mode & S_IROTH) > 0) printf("r");
        else printf("-");

        if((file_stat -> st_mode & S_IWOTH) > 0) printf("w");
        else printf("-");

        if((file_stat -> st_mode & S_IXOTH) > 0) printf("x\n");
        else printf("-\n");

        printf("Last modified: %s \n", ctime(&file_stat -> st_mtime));
       }
        
    
}


int nftw_fun(const char *abs_path, const struct stat *file_stat, int file_flag, struct FTW *ftw){
    if(file_flag == FTW_F){
        display_file_stats(abs_path, file_stat);
    }
    return 0;
}


int main(int argc, char **argv){
// ./a.out /home/patryk/Desktop/sysopy -1 2018 3 18 18 50 50

    struct tm tm_time;

    if(argc != 9){
        err_sys("Wrong number of agruments!");
    }

    int last_index = 0;

    while(argv[1][last_index] != '\0') last_index++;

    if(argv[1][last_index - 1] == '/') argv[1][last_index - 1] = '\0';

    char* dir_name = argv[1];

    if(argv[1][0] != '/'){
        char cwd[1024];
        if(getcwd(cwd, sizeof(cwd)) == NULL){
            err_sys("Coulnd't get cwd!");
        };
        strcat(cwd, "/");
        strcat(cwd, dir_name);
        dir_name = cwd;
    }


    if(atoi(argv[2]) < -1 || atoi(argv[2]) > 1){
        err_sys("Wrong comparison mode!");
    }

    if(atoi(argv[3]) < 1900){
        err_sys("Wrong year!");
    }

    if(atoi(argv[4]) < 1 || atoi(argv[4]) > 12){
        err_sys("Wrong month!");
    }

    if(atoi(argv[5]) < 1 || atoi(argv[5]) > 31){
        err_sys("Wrong day!");
    }

    if(atoi(argv[6]) < 0 || atoi(argv[6]) > 23){
        err_sys("Wrong hour!");
    }

    if(atoi(argv[7]) < 0 || atoi(argv[7]) > 59){
        err_sys("Wrong minute!");
    }

    if(atoi(argv[8]) < 0 || atoi(argv[8]) > 59){
        err_sys("Wrong second!");
    }

    tm_time.tm_year = atoi(argv[3]) - 1900;
    tm_time.tm_mon = atoi(argv[4]) - 1;
    tm_time.tm_mday = atoi(argv[5]);
    tm_time.tm_hour = atoi(argv[6]);
    tm_time.tm_min = atoi(argv[7]);
    tm_time.tm_sec = atoi(argv[8]);
    tm_time.tm_isdst = 0;

    comp_date = mktime(&tm_time);
    comp_mode = atoi(argv[2]);

    nftw(dir_name, nftw_fun, 20, FTW_PHYS);

    return 0;
}