
CC = gcc

progs = forkit tryit

CFLAGS = -Wall -g -pedantic -Wextra 

LDFLAGS = -Wall -g -pedantic -Wextra  

all: $(progs)

forkit: forkit.o 
	$(CC) -o forkit $(LDFLAGS) forkit.o

forkit.o: forkit.c
	$(CC) $(CFLAGS) -c forkit.c

tryit: tryit.o 
	$(CC) -o tryit $(LDFLAGS) tryit.o

tryit.o: tryit.c
	$(CC) $(CFLAGS) -c tryit.c

clear:
	rm -f *.o *~
