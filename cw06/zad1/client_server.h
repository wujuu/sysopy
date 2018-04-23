#ifndef CLIENT_SERVER
#define CLIENT_SERVER


#define MSG_SIZE 512

#define public_key_id 0
#define public_key_path "/home/patryk/XSI_keys/keys.txt"

#define MIRROR 1
#define CALC 2
#define TIME 3
#define END 4

struct msg{
    long type;
    char txt[MSG_SIZE];
};

struct msg init_msg(long new_type, char* new_txt);
void print_queue_stats(int queue_id);

#endif