SOURCE=time_test
CFLAGS=-Wall -O
LIBPATH=/home/patryk/Desktop/sysopy/projekt1/libraries



shared_lib:
	gcc -I$(LIBPATH) $(SOURCE).c -o $(SOURCE)_shared_ver -L$(LIBPATH) -lsblocks -ldblocks -Wl,-rpath,$(LIBPATH)

static_lib:
	gcc -static -I$(LIBPATH) $(SOURCE).c -o $(SOURCE)_static_ver -L$(LIBPATH) -lsblocks -ldblocks
