#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h> 

#include "decoder.h"
#include "huffman.h"

void usage( char *name){
  printf("usage: %s [(infile|-)(outfile]]\n", name);
  exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]){
  int infile, outfile;
  node *tree;
  int *freq; 

 
  switch(argc) {

    case 1:
      outfile = STDOUT_FILENO;
      infile = STDIN_FILENO;
      break;
    case 2:
        if( !strcmp(argv[1], "-")){
            infile = STDIN_FILENO; 
	    } else {  
           if( -1 == (infile = open(argv[1], O_RDONLY))){ 
               perror(argv[1]); 
               exit(EXIT_FAILURE);
	    } 
	 }
       outfile = STDOUT_FILENO;
       break; 
     case 3:   
       if( !strcmp(argv[1], "-")){
             infile = STDIN_FILENO; 
       } else { 
             if( -1 == (infile = open(argv[1], O_RDONLY))){ 
               perror(argv[1]); 
               exit(EXIT_FAILURE);
	     } 
       }

       if( -1 == (outfile = 
            open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR))){ 
            perror(argv[2]); 
            exit(EXIT_FAILURE);
       }
       break;
  default: 
        usage(argv[0]);
        break;
  }


  
   
  
  freq = get_header(infile);
  

  
  tree = make_tree(freq); 
  
 
  decode_file(infile, outfile, tree);    
  
  free(freq);
  free_tree(tree); 
     
 
  if( infile != 0){
     if (-1 == close(infile))
        perror(argv[1]);
  }
  
  
  return 0;
}
