SOURCE=time_test
CFLAGS=-Wall -O
LIBPATH=../zad1
OUTPUT=raport2.txt
PARAMETERS=1000 1000 20000 1000 10000



all: 
	gcc -I$(LIBPATH) $(SOURCE).c ../zad1/dblocks.c ../zad1/sblocks.c -o $(SOURCE)
	echo '\nNON_LIBRARY_COMPILATION_STATS:\n' >> $(OUTPUT)
	echo "blocksSize, charsSize, closestTo, del -> add, del <-> add = $(PARAMETERS) \n" >> $(OUTPUT)
	./$(SOURCE) $(PARAMETERS) >> $(OUTPUT) 


clean:
	rm -r $(SOURCE) $(OUTPUT)

