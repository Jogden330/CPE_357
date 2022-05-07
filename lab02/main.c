#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

#include "readLongLine.h"

int main(int argc, char *argv[]){
  /*
    uses Read_long_line to get an arbatrally long sring from standeredin
    and prints it st standered out. then checks each line
    to see if the last line is the same as the next line
    if they are the same it dose not print it    
  */
  char *thisLine = read_long_line(stdin);
  char *nextLine = NULL;
  
  /* if the first line isnt only the 0 bit print it */
  if(thisLine[0] != '\0'){
        printf("%s\n",thisLine);
  }

  /* if there was 1 or fewer lines in the standered in free nextline */
  if(feof(stdin)){
        free(nextLine);
  }

  /* while not EOF read next line and compare them */
  while( !feof(stdin) ){
 
        nextLine = read_long_line(stdin);
    
        if(strcmp(thisLine, nextLine)){
              printf("%s\n", nextLine);
        } 
	
        free(thisLine);
        thisLine = nextLine;
    
    
  }
  
  
  free(thisLine);

  return 0;
}
