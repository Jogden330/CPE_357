
CC = gcc

CFLAGS = -Wall -g -pendantic -Wextra 

LDLAGS = -Wall -g -pendantic -Wextra

all: fw

fw: main.o readLongWord.o hash.o
	$(CC) -o fw $(LDFLAGS) main.o readLongWord.o hash.o

main.o: main.c readLongWord.h hash.h
	$(CC) $(CFFLAGS) -c main.c

readLongWord.o: readLongWord.c readLongWord.h
	$(CC) $(CFFLAGS) -c readLongWord.c

hash.o: hash.c hash.h
	$(CC) $(CFFLAGS) -c hash.c

clear:
	rm -f *.o *~
