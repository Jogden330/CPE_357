#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

#include "huffman.h"

int main(int argc, char *argv[]){
  FILE *infile;
  char *code[NUM_CHAR];
  int i;
   
  /* open the file if there is one or use standered in */
  if (argc > 1){
    if( !(infile = fopen(argv[1],"r"))){
      perror(argv[1]);
    }
   } else {
    infile = stdin;
  }
  
  /*make the htable*/
  make_htable(infile, code);
   
  /*print the htable*/ 
  for(i = 0; i < NUM_CHAR; i++){
    if(code[i])
      printf("0x%02x %s\n", i, code[i]);

  }
  /*free and close open files*/
  free_codes(code);
  if( infile && (infile != stdin) )
    fclose(infile);

  return 0;
}
