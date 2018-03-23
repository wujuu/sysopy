#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>

int MAX_ARG_SIZE = 20;


//DISPLAYING SINGLE COMMAND BY INDEX
void display_command(char ***commands, int index){
    int j = 0;
    
    for(j = 0; j < MAX_ARG_SIZE && commands[index][j] != NULL; j++)
        printf("%s ", commands[index][j]);

    printf("\n");
}


char ***parse_commands(char *commands_file_path, int *commands_amount){

    struct stat commands_stat;
    int commands_size, i, j;
    char *r_buff;
    FILE *commands_ptr;
    char * full_command, *sub_command;
    char *** commands;


    *commands_amount = 0;


    //READING FILE STATS
    if(stat(commands_file_path, &commands_stat) == -1){
        perror("Failed to get file stats");
        exit(1);
    }

    commands_size = commands_stat.st_size;
    

    //READING FILE
    r_buff = malloc(commands_size);

    commands_ptr = fopen(commands_file_path, "r");

    if(commands_ptr == NULL){
        perror("Open commands file failed!");
        exit(1);
    }

    if(fread(r_buff, sizeof(char), commands_size, commands_ptr) != commands_size){
        perror("Failed to read from file!");
        exit(1);
    }

    fclose(commands_ptr);



    //COUNTING AMOUNT OF COMMANDS IN THE FILE
    for(i = 0; i < commands_size; i++){
        if(r_buff[i] == '\n'){
            (*commands_amount)++;
        }
    }

    //ALLOCATING ENOUGH SPACE FOR COMMAND ARRAY
    commands = malloc(((*commands_amount) + 1) * sizeof(char**));


    //ALLOCATING AND INITIALIZING COMMAND ARRAY
    for(i = 0; i < (*commands_amount) + 1; i++){

        commands[i] = malloc(MAX_ARG_SIZE * sizeof(char*));

        for(j = 0; j < MAX_ARG_SIZE; j++){
            commands[i][j] = NULL;
        }
    }

    

    //SPLITTING RAW FILE DATA INTO COMMAND ARRAY
    i = 0;
    while((full_command = strsep(&r_buff, "\n")) ){
        j = 0;
        while((sub_command = strsep(&full_command, " "))){
            commands[i][j] = sub_command;
            j++;
        }
        i++;
    }



    return commands;
}


int main(int argc, char **argv){


    int commands_amount;

    //PARSING COMMANDS INTO NICE COMMAND ARRAY
    char ***commands = parse_commands(argv[1], &commands_amount);


    //MAIN INTERPRETER LOOP
    for(int i = 0; i < commands_amount; i++){
        pid_t pid;

        //FORKING
        if((pid = vfork()) < 0){
            perror("Fork failed!");
        }
        else if(pid == 0){  //IF CHILD, EXECUTE COMMAND

            printf("Command: ");
            display_command(commands, i);

            if (execvp(commands[i][0], commands[i])){
                perror("Failed to execute command!");
                exit(1);
            }
        }
        else if(pid > 0){   //IF PARENT, WAIT FOR CHILD TO EXECUTE COMMAND
            int statloc;

            wait(&statloc);

            if(statloc != 0){
                exit(0);
            }

            printf("\n");     
        }
    }
    exit(0);
}