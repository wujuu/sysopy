CFLAGS=-Wall -Os

fork_dir_mgmt: fork_dir_mgmt.o
	gcc $(CFLAGS) fork_dir_mgmt.o -o fork_dir_mgmt

fork_dir_mgmt.o: fork_dir_mgmt.c
	gcc $(CFLAGS) -c fork_dir_mgmt.c -o fork_dir_mgmt.o

clean:
	rm *.o fork_dir_mgmt