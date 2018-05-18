CFLAGS=-Wall -Os

filter_image: filter_image.o
	gcc $(CFLAGS) filter_image.o -o filter_image -lm -lpthread

filter_image.o: filter_image.c
	gcc $(CFLAGS) -c filter_image.c -o filter_image.o 
clean:
	rm *.o filter_image