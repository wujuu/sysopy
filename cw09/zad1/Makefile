CFLAGS=-Wall -Os

all:
	make production_line

production_line: production_line.o
	gcc $(CFLAGS) production_line.o -lpthread -o production_line

production_line.o: production_line.c
	gcc $(CFLAGS) -c production_line.c -lpthread -o production_line.o 

clean:
	rm *.o production_line