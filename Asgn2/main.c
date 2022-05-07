#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<ctype.h>

#include "readLongWord.h"
#include "hash.h"

void usage( char *name, table *hashtable){
  printf("usage: %s [-n num] [ file1 [ file 2 ... ]]\n", name);
  free_table(hashtable);
  exit(EXIT_FAILURE);
}

void tohash(FILE *infile, table *hashtable){
   char *word;
   /* reads a word from infile and returns adds it to the 
      hastable then closses infile */
    while(!feof(infile)){
      if(word = read_long_word(infile) ){
          insert(word, hashtable);
	  rehash(hashtable);
     }
   }
   
   if(infile){
     fclose(infile);
   }

}

int main(int argc, char *argv[]) {
 
  int i, n, flag;
  FILE *infile;
  char *end;
  
  
  table *hashtable;
  
  infile = NULL;
  
  n = 10;
  flag = 0;

  /* prsses the agument to see waht sould be done
     if the agument is bad show usage message
  */
  if( !(hashtable = make_table()) ){
      perror(argv[i]);
      exit(EXIT_FAILURE);
  }

  for(i=1; i < argc; i++){

    if ( !strcmp(argv[i],"-n") ){
      i++;
      /* convert argv[i+1] to a number */
      if(i >= argc){
	usage(argv[0], hashtable);
      }
      
      n = strtol(argv[i],&end,10);

      if ( *end != '\0' || n < 0 ) {
        usage(argv[0], hashtable);
	
      }
      
    } else {

    /* use argv[i] as file path and open */
      flag = 1;

      if(infile = fopen(argv[i],"r")){
        tohash(infile, hashtable);
       
      } else {
        perror(argv[i]);
      }
   }    
 }
  /* if the aguments were not a file use standered in */
  if(flag ==0)
        tohash(stdin, hashtable);

  /* print to stndered out and free hashtable */
  
  print_n_table(hashtable, n);  
  
  free_table(hashtable);
   
  return 0;
}
