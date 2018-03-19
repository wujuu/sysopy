CFLAGS=-Wall -Os

nftw_dir_mgmt: nftw_dir_mgmt.o
	gcc $(CFLAGS) nftw_dir_mgmt.o -o nftw_dir_mgmt

nftw_dir_mgmt.o: nftw_dir_mgmt.c
	gcc $(CFLAGS) -c nftw_dir_mgmt.c -o nftw_dir_mgmt.o

clean:
	rm *.o nftw_dir_mgmt