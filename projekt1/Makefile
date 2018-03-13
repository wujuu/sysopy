SOURCE=time_test
CFLAGS=-Wall -Os
LIBPATH=/home/patryk/Desktop/sysopy/projekt1/libraries
OUTPUT=raport3b.txt



shared_lib:
	gcc -I$(LIBPATH) $(SOURCE).c -o $(SOURCE)_shared_ver -L$(LIBPATH) -lsblocks -ldblocks -Wl,-rpath,$(LIBPATH)
	echo "\nGCC OPTIONS: $(CFLAGS), SHARED_LIB_STATS: \n" >> $(OUTPUT)
	./$(SOURCE)_shared_ver 1000 1000 20000 1000 100000 >> $(OUTPUT)

static_lib:
	gcc -static -I$(LIBPATH) $(SOURCE).c -o $(SOURCE)_static_ver -L$(LIBPATH) -lsblocks -ldblocks
	echo "\nGCC OPTIONS: $(CFLAGS), STATIC_LIB_STATS: \n" >> $(OUTPUT)
	./$(SOURCE)_static_ver 1000 1000 20000 1000 100000 >> $(OUTPUT)
