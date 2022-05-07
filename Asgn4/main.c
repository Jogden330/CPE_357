#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<ctype.h>
#include <sys/types.h> 
#include <sys/stat.h>  
#include <fcntl.h>


#include "Arch.h"

#define OFF_SET 3



void usage(const char *name) {
  fprintf(stderr,"usage: %s [ctxvS]f tarfile [ path [... ] ]\n",name);
  exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
  char *flags, *tarfile, *path;
  int c, t ,x, v, f, S, numPaths;
  int infile, outfile;
  int i;

  c = 0;
  t = 0;
  x = 0;
  v = 0;
  f = 0;
  S = 0;
  numPaths = 0;
 

  if( (argc >= 2) ){
  
    flags = argv[1];
    tarfile = argv[2];
  
    if( argc >= 3)
      numPaths = argc - OFF_SET; /* store the number of paths */

  } else {
     usage(argv[0]);
  }
  
  /* check the first argument for the flages if one of them is not in
     the flags print a usage message and quit
  */
  for (i = 0; i < strlen(flags); i++){

    switch(flags[i]){

      case 'c':
        c++; 
        break;
      case 't':
        t++; 
        break;
      case 'x':
        x++; 
        break;
      case 'v':
        v++; 
        break;
      case 'f':
        f++; 
        break;
      case 'S':
        S++; 
        break;
      default :
       usage(argv[0]);
    }     

  }
  if(!f)
    usage(argv[0]);

  if(t){
    if(x||c)
       usage(argv[0]);

     if( -1 == (infile = open(tarfile, O_RDONLY))){ 
          perror(tarfile); 
          exit(EXIT_FAILURE);
        }
     if(argc < 4){    
        print_table(infile, v);
     } else {
       
        for (i = 0; i < numPaths; i++){
        /* do somthing for each path */
	 
	 path = argv[(OFF_SET+i)];
         
         print_table_name(infile, path, v);
         lseek(infile, 0, SEEK_SET);

	}
     }
  } else if(x){
      if(c)
         usage(argv[0]);
       if( -1 == (infile = open(tarfile, O_RDONLY))){ 
          perror(tarfile); 
          exit(EXIT_FAILURE);
        }
       
        archive_get(infile,v);
  } else  if(c){
     if( -1 == (outfile = 
	     open(tarfile,  O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR))){ 
      perror(tarfile); 
      
    } else if(numPaths){
      for (i = 0; i < numPaths; i++){
        /* do somthing for each path */
        path = argv[(OFF_SET+i)];
        if( -1 == (infile = open(path, O_RDONLY))){ 
          perror(path); 
        } else {
          archive_creator(infile, outfile, path, S, v);

       }
    }
    pad_ending(outfile); 
   } 
  }

  return 0;
}
