CFLAGS=-Wall -Os


file_mgmt: file_mgmt.o
	gcc $(CFLAGS) file_mgmt.o -o file_mgmt

file_mgmt.o: file_mgmt.c
	gcc $(CFLAGS) -c file_mgmt.c -o file_mgmt.o

clean:
	rm *.o file_mgmt