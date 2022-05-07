#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h> 

#include "huffman.h"
#include "encoder.h"


void usage( char *name){
  printf("usage: %s infile  [ outfile ] \n", name);
  exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]){
  int infile, outfile, numChars;
  char *code[NUM_CHAR];
  int *freq;
   
  /* open the file if there is one or use standered in */
  if (argc == 2 ||  argc == 3){
    if( -1 == (infile = open(argv[1], O_RDONLY))){
      perror(argv[1]);
      exit(EXIT_FAILURE);
    }
    if (argc == 3){
      if( -1 == (outfile =
      open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR))){
         perror(argv[1]);
         exit(EXIT_FAILURE);
      }
    } else {
      outfile = STDOUT_FILENO;
    }
    
  } else {
      usage(argv[0]);
  } 
  
  freq = char_freq(infile);
   
  write_header(outfile, freq);
   
  /* make the htable */
  numChars = make_htable(freq, code);
   
  
  encode_file(infile, outfile, code, numChars);
  /*free and close open files*/
  free(freq);  
  
  free_codes(code);
 
  if( infile != 0){
     if (-1 == close(infile))
        perror(argv[1]);
  }
  
  return 0;
}
