
CC = gcc

CFLAGS = -Wall -g -pedantic -Wextra -I ~pn-cs357/Given/Talk 

LDFLAGS = -Wall -g -pedantic -Wextra -L ~pn-cs357/Given/Talk/lib64 

all: mytalk

mytalk: main.o chat.o
	$(CC) -o mytalk $(LDFLAGS) main.o chat.o -ltalk -lncurses

main.o: main.c chat.h
	$(CC) $(CFLAGS) -c main.c

chat.o: chat.c chat.h
	$(CC) $(CFLAGS) -c chat.c 

clear:
	rm -f *.o *~
