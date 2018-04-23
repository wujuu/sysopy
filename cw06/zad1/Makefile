CFLAGS=-Wall -Os

system_v_queues: system_v_queues.o
	gcc $(CFLAGS) system_v_queues.o -o system_v_queues

system_v_queues.o: system_v_queues.c
	gcc $(CFLAGS) -c system_v_queues.c -o system_v_queues.o

clean:
	rm *.o system_v_queues