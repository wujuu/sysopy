#include<stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <dirent.h>
#include <string.h>

void err_sys(const char *x) 
{ 
    perror(x); 
    exit(1); 
}



void display_file_stats(struct stat file_stat){
    printf("File size in bytes: %li \n", file_stat.st_size);
    printf("File permissions: ");

    if((file_stat.st_mode & S_IRUSR) > 0) printf("r");
    else printf("-");

    if((file_stat.st_mode & S_IWUSR) > 0) printf("w");
    else printf("-");

    if((file_stat.st_mode & S_IXUSR) > 0) printf("x");
    else printf("-");

    if((file_stat.st_mode & S_IRGRP) > 0) printf("r");
    else printf("-");

    if((file_stat.st_mode & S_IWGRP) > 0) printf("w");
    else printf("-");

    if((file_stat.st_mode & S_IXGRP) > 0) printf("x");
    else printf("-");

    if((file_stat.st_mode & S_IROTH) > 0) printf("r");
    else printf("-");

    if((file_stat.st_mode & S_IWOTH) > 0) printf("w");
    else printf("-");

    if((file_stat.st_mode & S_IXOTH) > 0) printf("x\n");
    else printf("-\n");

    printf("Last modified: %s \n", ctime(&file_stat.st_mtime));

}

void explore_dir(const char *dir_name, int comp_mode, time_t comp_date){

    DIR *dir = opendir(dir_name);

    if(dir == NULL){
        err_sys("Open dir failed!");
    }

    struct dirent *file = readdir(dir);

    while(file != NULL){
        char abs_path[1024];
        strcpy(abs_path, dir_name);
        strcat(abs_path, "/");
        strcat(abs_path, file -> d_name);

        if(strcmp(file -> d_name, ".") != 0 && strcmp(file -> d_name, "..") != 0){
            
            struct stat file_stat; 

            stat(abs_path, &file_stat);

            if(S_ISDIR(file_stat.st_mode)){
                explore_dir(abs_path, comp_mode, comp_date);
            }
            else if (S_ISREG(file_stat.st_mode)){
                if(comp_mode == -1 && comp_date < file_stat.st_mtime){
                    printf("Absolute path: %s\n", abs_path);
                    display_file_stats(file_stat);
                }
                else if (comp_mode == 0 && comp_date == file_stat.st_mtime){
                    printf("Absolute path: %s\n", abs_path);
                    display_file_stats(file_stat);
                }
                else if(comp_mode == 1 && comp_date > file_stat.st_mtime){
                    printf("Absolute path: %s\n", abs_path);
                    display_file_stats(file_stat);
                }    
            }
        }

        file = readdir(dir);
    }

    closedir(dir);
}



int main(int argc, char **argv){
// -1 2017 3 15 5 50 50
    struct tm tm_time;
    //time_t raw_time;

    tm_time.tm_year = atoi(argv[3]) - 1900;
    tm_time.tm_mon = atoi(argv[4]) - 1;
    tm_time.tm_mday = atoi(argv[5]);
    tm_time.tm_hour = atoi(argv[6]);
    tm_time.tm_min = atoi(argv[7]);
    tm_time.tm_sec = atoi(argv[8]);
    tm_time.tm_isdst = 0;

    explore_dir(argv[1], atoi(argv[2]), mktime(&tm_time));

    return 0;
}