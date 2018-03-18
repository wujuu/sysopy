CFLAGS=-Wall -Os

cmd_file_mgmt: cmd_file_mgmt.o file_mgmt.o 
	gcc $(CFLAGS) cmd_file_mgmt.o file_mgmt.o -o cmd_file_mgmt

file_mgmt.o: file_mgmt.c file_mgmt.h
	gcc $(CFLAGS) -c file_mgmt.c -o file_mgmt.o

cmd_file_mgmt.o: cmd_file_mgmt.c
	gcc $(CFLAGS) -c cmd_file_mgmt.c -o cmd_file_mgmt.o

clean:
	rm *.o cmd_file_mgmt