SHELL = /bin/bash

PROGS = timeit

CC = gcc

CFLAGS = -ansi -Wall -pedantic -Wextra -g -O4

LDFLAGS = -ansi -Wall -pedantic -Wextra -g

all: $(PROGS)

timeit: timeit.o
	$(CC) -o timeit $(LDFLAGS) timeit.o

timeit.o: timeit.c
	$(CC) $(CFFLAGS) -c timeit.c

clear:
	rm -f *.o *~
