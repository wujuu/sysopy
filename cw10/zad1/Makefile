CFLAGS=-Wall -Os

all:
	make server
	make client

server: server.o info.o
	gcc $(CFLAGS) server.o info.o -o server -lpthread

server.o: server.c info.h
	gcc $(CFLAGS) -c server.c -o server.o 

client: client.o info.o 
	gcc $(CFLAGS) client.o info.o -o client

client.o: client.c info.h
	gcc $(CFLAGS) -c client.c -o client.o

info.o: info.c info.h
	gcc $(CFLAGS) -c info.c -o info.o

clean:
	rm *.o server client