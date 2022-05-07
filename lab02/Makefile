CC = gcc

CFLAGS = -Wall - g -pendantic -Wextra -O4

LDLAGS = -Wall -g -pendantic -Wextra

uniq: main.o readLongLine.o
	$(CC) -o uniq $(LDFLAGS) main.o readLongLine.o

main.o: main.c readLongLine.h
	$(CC) $(CFFLAGS) -c main.c

readLongLine.o: readLongLine.c readLongLine.h
	$(CC) $(CFFLAGS) -c readLongLine.c

clear:
	rm -f *.0 *~
