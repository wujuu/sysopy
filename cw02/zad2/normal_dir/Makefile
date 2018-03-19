CFLAGS=-Wall -Os

normal_dir_mgmt: normal_dir_mgmt.o
	gcc $(CFLAGS) normal_dir_mgmt.o -o normal_dir_mgmt

normal_dir_mgmt.o: normal_dir_mgmt.c
	gcc $(CFLAGS) -c normal_dir_mgmt.c -o normal_dir_mgmt.o

clean:
	rm *.o normal_dir_mgmt