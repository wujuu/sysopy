CFLAGS=-Wall -Os

all:
	make barber
	make client_batch

barber: barber.o clients_queue.o single_sem.o barbershop.o
	gcc $(CFLAGS) barber.o clients_queue.o single_sem.o barbershop.o -o barber

client_batch: client_batch.o clients_queue.o single_sem.o client_batch.o
	gcc $(CFLAGS) client_batch.o clients_queue.o single_sem.o barbershop.o -o client_batch

barber.o: barber.c
	gcc $(CFLAGS) -c barber.c -o barber.o

client_batch.o: client_batch.c
	gcc $(CFLAGS) -c client_batch.c -o client_batch.o

clients_queue.o: clients_queue.c clients_queue.h
	gcc $(CFLAGS) -c clients_queue.c -o clients_queue.o

single_sem.o: single_sem.c single_sem.h
	gcc $(CFLAGS) -c single_sem.c -o single_sem.o

barbershop.o: barbershop.c barbershop.h
	gcc $(CFLAGS) -c barbershop.c -o barbershop.o

clean:
	rm *.o barber client_batch