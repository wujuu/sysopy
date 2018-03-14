CFLAGS = -O -Wall
LIB = sblocks

static:

	gcc -c $(LIB).c -o $(LIB).o

	ar cr lib$(LIB).a $(LIB).o

	make clean

shared:

	gcc -fPIC -c $(LIB).c -o $(LIB).o
	gcc -fPIC -shared -o lib$(LIB).so $(LIB).o

	make clean

clean:
	rm -f *.o

