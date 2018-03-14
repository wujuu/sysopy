SOURCE=time_test
CFLAGS=-Wall -Os
LIBPATH=../zad1
OUTPUT=raport3b.txt
PARAMETERS=1000 1000 20000 1000 1000



shared_ver:
	gcc -I$(LIBPATH) $(SOURCE).c -o $(SOURCE)_shared_ver -L$(LIBPATH) -lsblocks -ldblocks -Wl,-rpath,$(LIBPATH)
	echo "\nGCC OPTIONS: $(CFLAGS), SHARED_LIB_STATS: \n" >> $(OUTPUT)
	echo "blocksSize, charsSize, closestTo, del -> add, del <-> add = $(PARAMETERS) \n" >> $(OUTPUT)
	./$(SOURCE)_shared_ver $(PARAMETERS) >> $(OUTPUT)

static_ver:
	gcc -static -I$(LIBPATH) $(SOURCE).c -o $(SOURCE)_static_ver -L$(LIBPATH) -lsblocks -ldblocks
	echo "\nGCC OPTIONS: $(CFLAGS), STATIC_LIB_STATS: \n" >> $(OUTPUT)
	echo "blocksSize, charsSize, closestTo, del -> add, del <-> add = $(PARAMETERS) \n" >> $(OUTPUT)
	./$(SOURCE)_static_ver $(PARAMETERS) >> $(OUTPUT)

dynamic_ver:
	gcc -I$(LIBPATH) dyn_$(SOURCE).c -o dyn_$(SOURCE) -ldl
	echo "\nGCC OPTIONS: $(CFLAGS), DYNAMIC_LOADING__STATS: \n" >> $(OUTPUT)
	echo "blocksSize, charsSize, closestTo, del -> add, del <-> add = $(PARAMETERS) \n" >> $(OUTPUT)
	./dyn_$(SOURCE) $(PARAMETERS) >> $(OUTPUT)
clean:
	rm -r $(SOURCE)_shared_ver $(SOURCE)_static_ver dyn_$(SOURCE) $(OUTPUT)
