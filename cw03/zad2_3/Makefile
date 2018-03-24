CFLAGS=-Wall -Os

interpreter: interpreter.o cpu_lim_test.o vmem_lim_test.o
	gcc $(CFLAGS) interpreter.o -o interpreter
	gcc $(CFLAGS) cpu_lim_test.o -o cpu_lim_test
	gcc $(CFLAGS) vmem_lim_test.o -o vmem_lim_test

interpreter.o: interpreter.c cpu_lim_test.c vmem_lim_test.c
	gcc $(CFLAGS) -c interpreter.c -o interpreter.o
	gcc $(CFLAGS) -c cpu_lim_test.c -o cpu_lim_test.o
	gcc $(CFLAGS) -c vmem_lim_test.c -o vmem_lim_test.o

clean:
	rm *.o interpreter vmem_lim_test cpu_lim_test