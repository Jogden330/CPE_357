#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  int i, end;
  if ( argc != 2 ) {
    fprintf(stderr,"usage: %s num\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  end = atoi(argv[1]);
  for(i=0;i<end;i++)
    putchar(i % 256);
  return 0;
}
