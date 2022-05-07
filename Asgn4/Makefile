SHELL = /bin/bash

PROGS = mytar

CC = gcc

CFLAGS = -Wall -pedantic -Wextra -g -O4

LDFLAGS = -Wall -pedantic -g

all: $(PROGS)

mytar: main.o Arch.o  
	$(CC) -o mytar $(LDFLAGS) main.o Arch.o   

main.o: main.c  Arch.h
	$(CC) $(CFLAGS) -c main.c

Arch.o: Arch.c  Arch.h
	$(CC) $(CFLAGS) -c Arch.c


clear:
	rm -f *.o *~
