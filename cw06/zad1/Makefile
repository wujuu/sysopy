CFLAGS=-Wall -Os

all:
	make server
	make client

server: server.o client_server.o
	gcc $(CFLAGS) server.o client_server.o -o server

server.o: server.c client_server.h
	gcc $(CFLAGS) -c server.c -o server.o 

client: client.o client_server.o 
	gcc $(CFLAGS) client.o client_server.o -o client

client.o: client.c client_server.h
	gcc $(CFLAGS) -c client.c -o client.o

client_server.o: client_server.c client_server.h
	gcc $(CFLAGS) -c client_server.c -o client_server.o

clean:
	rm *.o server client