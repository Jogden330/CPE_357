#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <mush.h>
#include <signal.h>


#include "much2.h"

char PMT[] = "8-P ";


void usage(const char *name) {
  fprintf(stderr,"usage: %s [input file]\n",name);
  exit(EXIT_FAILURE);
}

int main( int argc, char *argv[] )  {

  FILE* infile;
  char *line;
  pipeline cl;  
 
   
  /* is there a to few argumants print a usage message and exit */
  if(argc > 2) {
    usage(argv[0]);
  }

  /* Set infile to open a read file or stdin */
  if(argc == 2){

    if( NULL  == (infile = fopen(argv[1], "r"))){
      perror(argv[1]);
      exit(EXIT_FAILURE);
    }


  } else {

    infile = stdin;
  }

  do{
  

    if(argc != 2){
      printf("%s", PMT);
    }

    
    if( NULL != (line = readLongString(infile))){  
      cl = crack_pipeline(line);
      /* print_pipeline(stdout, cl); */
      /*printf("%s \n" ,cl->stage->argv[0]);*/
      if(!strcmp(cl->stage->argv[0], "cd")){
	cd_cmd(cl->stage);
         
      } else {
        /*piplin gose here */
        pipeline_cmd(cl);
      }
      free(cl->cline);
      free_pipeline(cl);
            
      
    }


  } while(!feof(infile));

  if(argc == 2) {
    fclose(infile);
  } 
  
   
  return 0;
}
