CFLAGS=-Wall -Os

all:
	make master
	make slave

master: master.o
	gcc $(CFLAGS) master.o -o master

master.o: master.c
	gcc $(CFLAGS) -c master.c -o master.o

slave: slave.o
	gcc $(CFLAGS) slave.o -o slave

slave.o: slave.c
	gcc $(CFLAGS) -c slave.c -o slave.o

clean:
	rm *.o master slave