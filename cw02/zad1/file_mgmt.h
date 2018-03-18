#ifndef FILE_MGMT_H
#define FILE_MGMT_H

void err_sys(const char *x);
void fill_random(char *to_fill, int size);
char *get_random(int size);
void generate(const char *file_name, int records, int bytes);
void sort(const char *file_name, int records, int bytes, int if_sys);
void copy(const char *orig_file_name, const char* new_file_name, int records, int bytes, int if_sys);


#endif