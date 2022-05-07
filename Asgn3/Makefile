SHELL = /bin/bash

PROGS = hencode hdecode

CC = gcc

CFLAGS = -ansi -Wall -pedantic -Wextra -g -O4

LDFLAGS = -ansi -Wall -pedantic -Wextra -g

all: $(PROGS)

hencode: hencode.o huffman.o encoder.o
	$(CC) -o hencode $(LDFLAGS) hencode.o huffman.o encoder.o

hencode.o: hencode.c huffman.h encoder.h
	$(CC) $(CFLAGS) -c hencode.c

huffman.o: huffman.c huffman.h
	$(CC) $(CFLAGS) -c huffman.c

encoder.o: encoder.c encoder.h huffman.h
	$(CC) $(CFLAGS) -c encoder.c 

hdecode: hdecode.o decoder.o huffman.o
	$(CC) -o hdecode $(LDFLAGS) hdecode.o decoder.o huffman.o

hdecode.o: hdecode.c decoder.h huffman.h
	$(CC) $(CFLAGS) -c hdecode.c

decoder.o: decoder.c decoder.h huffman.h 
	$(CC) $(CFLAGS) -c decoder.c 


clear:
	rm -f *.o *~
