SHELL = /bin/bash

CC = gcc

CFLAGS = -ansi -Wall -pedantic -Wextra -g -O4

LDFLAGS = -ansi -Wall -pedantic -Wextra -g

htable: main.o huffman.o
	$(CC) -o htable $(LDFLAGS) main.o huffman.o

main.o: main.c huffman.h
	$(CC) $(CFLAGS) -c main.c

huffman.o: huffman.c huffman.h
	$(CC) $(CFLAGS) -c huffman.c

clear:
	rm -f *.0 *~
