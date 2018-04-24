#ifndef CLIENT_SERVER_H
#define CLIENT_SERVER_H


#define MSG_SIZE 512

#define MAX_CLIENTS 5

#define public_key_id 0
#define public_key_path "/home/patryk/XSI_keys/keys.txt"


#define MIRROR 1
#define CALC 2
#define TIME 3
#define END 4
#define START 5
#define STOP 6


struct msg{
    long type;
    char txt[MSG_SIZE];
};

void fill_nulls(char *string);
struct msg init_msg(long new_type, char* new_txt);
void print_queue_stats(int queue_id);
void remove_trailing_newline(char* string);
#endif