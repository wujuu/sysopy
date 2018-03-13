CFLAGS= -Wall -O
LIBPATH = /home/patryk/Desktop/sysopy/projekt1/libraries


shared_lib:
	gcc -I$(LIBPATH) my_lib_test.c -o my_shared_lib_test -L$(LIBPATH) -lsblocks -ldblocks -Wl,-rpath,$(LIBPATH)

static_lib:
	gcc -static -I$(LIBPATH) my_lib_test.c -o my_static_lib_test -L$(LIBPATH) -lsblocks -ldblocks
