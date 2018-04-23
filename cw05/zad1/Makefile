CFLAGS=-Wall -Os

pipe_interpreter: pipe_interpreter.o
	gcc $(CFLAGS) pipe_interpreter.o -o pipe_interpreter

pipe_interpreter.o: pipe_interpreter.c
	gcc $(CFLAGS) -c pipe_interpreter.c -o pipe_interpreter.o

clean:
	rm *.o pipe_interpreter