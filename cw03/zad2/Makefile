CFLAGS=-Wall -Os

interpreter: interpreter.o
	gcc $(CFLAGS) interpreter.o -o interpreter

interpreter.o: interpreter.c
	gcc $(CFLAGS) -c interpreter.c -o interpreter.o

clean:
	rm *.o interpreter