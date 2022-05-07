CC = gcc

CFLAGS = -Wall -g -pedantic -Wextra -I ~pn-cs357/Given/Mush/include

LDFLAGS = -Wall -g -pedantic -Wextra -L ~pn-cs357/Given/Mush/lib64

all: mush2

mush2: main.o much2.o
	$(CC) -o much2 $(LDFLAGS) main.o much2.o -lmush 

main.o: main.c much2.h 
	$(CC) $(CFLAGS) -c main.c

much2.o: much2.c much2.h
	$(CC) $(CFLAGS) -c much2.c

clear:
	rm -f *.o *~
