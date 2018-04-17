#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <string.h>

int MAX_ARG_SIZE = 20;

int **make_pipes(int pipes_amount){

    int **pipes = malloc(pipes_amount * sizeof(int*));
    
    for(int i = 0; i < pipes_amount; i++){
        pipes[i] = malloc(2 * sizeof(int));
        pipe(pipes[i]);
    }

    return pipes;
}

void close_pipes(int **pipes, int pipes_amount){
    for(int i = 0; i < pipes_amount; i++){
        close(pipes[i][0]);
        close(pipes[i][1]);
    }
}


void interpret_line(char ***line_commands, int line_commands_amount){

    int pipes_amount = line_commands_amount - 1;
    int **pipes = make_pipes(pipes_amount);

    pid_t child_pid;

    //Initial command
    if((child_pid = fork()) < 0){
        perror("fork failed!");
        exit(1);
    }else if(child_pid == 0){

        if(dup2(pipes[0][1], STDOUT_FILENO) < 0){
            perror("dup2 failed!");
            exit(1);
        }

        close_pipes(pipes, pipes_amount);

        if(execvp(line_commands[0][0], line_commands[0]) < 0){
            perror("exec failed!");
            exit(1);
        }

        exit(0);
    }

    //Middle commands
    for(int i = 1; i < line_commands_amount - 1; i++){
        if((child_pid = fork()) < 0) {
            perror("fork failed!");
            exit(1);
        } else if(child_pid == 0) {

            if(dup2(pipes[i - 1][0], STDIN_FILENO) < 0){
                perror("dup2 failed!");
                exit(1);
            }

            if(dup2(pipes[i][1], STDOUT_FILENO) < 0){
                perror("dup2 failed!");
                exit(1);
            }

            close_pipes(pipes, pipes_amount);

            if(execvp(line_commands[i][0], line_commands[i]) < 0){
                perror("exec failed!");
                exit(1);
            }

            exit(0);
        }
    }

    //Last command
    if((child_pid = fork()) < 0){
        perror("fork failed!");
        exit(1);
    }else if(child_pid == 0){

        if(dup2(pipes[pipes_amount - 1][0], STDIN_FILENO) < 0){
            perror("dup2 failed!");
            exit(1);
        }

        close_pipes(pipes, pipes_amount);

        if(execvp(line_commands[line_commands_amount - 1][0], 
                  line_commands[line_commands_amount - 1]) < 0){
            perror("exec failed!");
            exit(1);
        }

        exit(0);
    }

    close_pipes(pipes, pipes_amount);
    waitpid(child_pid, NULL, 0);
}


//DISPLAYING SINGLE COMMAND BY INDEX
void display_command(char ***commands, int index){
    int j = 0;
    
    for(j = 0; j < MAX_ARG_SIZE && commands[index][j] != NULL; j++)
        printf("%s ", commands[index][j]);

    printf("\n");
}

void read_commands(char *commands_file_path){


    FILE *stream;
    char *line = NULL, *full_command = NULL, *sub_command = NULL;
    size_t len = 0;
    ssize_t nread;


    stream = fopen(commands_file_path, "r");

    if (stream == NULL) {
        perror("can't open this file!");
        exit(1);
    }

    while ((nread = getline(&line, &len, stream)) != -1) {

            int line_commands_amount = 0;

            for(int i = 0; i < nread; i++){
                if(line[i] == '|') line_commands_amount++;
            }

            line_commands_amount++;


            char *** line_commands = malloc((line_commands_amount + 1) * sizeof(char**));

            //ALLOCATING AND INITIALIZING COMMAND ARRAY
            for(int i = 0; i < line_commands_amount + 1; i++){

                line_commands[i] = malloc(MAX_ARG_SIZE * sizeof(char**));

                for(int j = 0; j < MAX_ARG_SIZE; j++){
                    line_commands[i][j] = NULL;
                }
            }


            
            int i = 0, j = 0;
            while((full_command = strsep(&line, "|")) ){

                j = 0;
                while((sub_command = strsep(&full_command, " "))){
                    
                    
                    if(strlen(sub_command) != 0){
                        if(sub_command[strlen(sub_command) - 1] == '\n') {
                            char *tmp_sub_command = malloc((strlen(sub_command) - 1) * sizeof(char));
                            for(int i = 0; i < strlen(sub_command) - 1; i++) tmp_sub_command[i] = sub_command[i];
                            line_commands[i][j] = tmp_sub_command;
                        }
                        else{
                            line_commands[i][j] = sub_command;
                        }
                        
                        j++;
                    }   
                }
                i++;
            }

            interpret_line(line_commands, line_commands_amount);
    }

    fclose(stream);

}



int main(){
    char *commands_path = "/home/patryk/Documents/IET/Sysopy/sysopy/cw05/zad1/commands.txt";

    read_commands(commands_path);

    exit(0);
}