#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<unistd.h>
#include<ctype.h>
#include "readLongWord.h"


#define STARTING_SIZE 42

char *read_long_word (FILE *file){
   /*
     takes in and returns a sring of arbatraly long size
     using dinamicly alocated memory
   */
     int count = 0;
     int size = STARTING_SIZE;
     char c;
     char *buffer;

     if(!file){
       perror("file");
       exit(EXIT_FAILURE);
     }

     /* initalizes the buffer  and checks to make sure it whent well */
     if( !(buffer = (char *)malloc(size)) ){
           perror("malloc");
           exit(EXIT_FAILURE);  
     }  
     
     /* reads each charicter from file and stors that to the buffer */
     while (isalpha(c = getc(file))){

           buffer[count++] = tolower(c);
	   
	   /* if the buffer is to small make it bigger */
	   if(count >= size){
	   	     size = 2*size;
		     if( !(buffer = realloc(buffer, size)) ){
		       perror("realloc");
                       exit(EXIT_FAILURE);
		     }

	   }	   
     }
     
     /* puts the ) bite at the end of the string */
     buffer[count] = '\0';
      
     /* chang the size of the buffer to be the lingth of the word */
     if( !(buffer = realloc(buffer, (count+2))) ){
		       perror("realloc");
                       exit(EXIT_FAILURE);
      }	       
    	    
     /* if the longth of the word is 0 free that space and return NULL */
      
     if((c == EOF && count == 0) || count == 0){
       free(buffer);
       return NULL;
     }
     
     return buffer;
     
}
