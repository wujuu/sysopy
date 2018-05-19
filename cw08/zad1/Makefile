CFLAGS=-Wall -Os

all:
	make generate_filter
	make example


generate_filter: generate_filter.o
	gcc $(CFLAGS) generate_filter.o -o generate_filter

generate_filter.o: generate_filter.c
	gcc $(CFLAGS) -c generate_filter.c -o generate_filter.o 

example: example.o filter_image.o
	gcc $(CFLAGS) example.o filter_image.o -o example -lm -lpthread

example.o: example.c 
	gcc $(CFLAGS) -c example.c -o example.o -lm -lpthread

clean:
	rm *.o generate_filter example