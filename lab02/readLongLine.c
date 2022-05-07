#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<unistd.h>
#include<ctype.h>
#include "readLongLine.h"


#define STARTING_SIZE 42

char *read_long_line (FILE *file){
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
     if( !(buffer = (char *)malloc(size*sizeof(char))) ){
           perror("malloc");
           exit(EXIT_FAILURE);  
     }  
     
     /* reads each charicter from file and stors that to the buffer */
     while ((c = getc(file)) != '\n' && (c != EOF)){

     	   buffer[count++] = c;
	   
	   /* if the buffer is to small make it bigger */
	   if(!(count < size)){
	   	     size = 2*size;
		     if( !(buffer = realloc(buffer, size*sizeof(char) )) ){
		       perror("realloc");
                       exit(EXIT_FAILURE);
		     }

	   }	   
     }
     
     /* puts the ) bite at the end of the string */
     buffer[count] = '\0';
     
     return buffer;
     
}
