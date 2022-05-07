#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<ctype.h>
#include <arpa/inet.h>
#include "huffman.h"
#include "encoder.h"

#define X8_BIT 8
#define X32_BIT 32

int write_header(int outfile, int *freq){
  
  int i, numchar;
  uint8_t numCharMinus1, c;
  uint32_t freqOf;
  
  numchar = 0;

  for (i = 0; i < NUM_CHAR; i++){
   
    if(freq[i])
      numchar++;
   
  }
  if(numchar){
    numCharMinus1 = numchar - 1;
    write(outfile, &numCharMinus1, sizeof(numCharMinus1));

  }
  /* for each chariter ad the charicter and its frequency to the bitstream */
  for (i = 0; i < NUM_CHAR; i++){
   
    if(freq[i]){
       
       c = i;
       freqOf = freq[i];
       write(outfile, &c, sizeof(c));
       freqOf= htonl(freqOf);
       write(outfile, &freqOf, sizeof(freqOf));
        
    } 
  }
   
  return numchar;
 
}

void encode_file(int infile, int outfile, char *codes[], int numChars){
  unsigned char c;
  char *Buffer, *CodeBuff;
  char *code;
  uint8_t buff;
  unsigned int poseInCode, poseInBuff, countBuff;
  int in, i;

  poseInBuff = 0;
  buff = 0;
  countBuff = 0;
  
  if( !(Buffer = (char *)malloc(sizeof(char)*(numChars + 1))) ){
    perror("malloc");
    exit(EXIT_FAILURE);
  }
    if( !(CodeBuff = (char *)malloc(sizeof(char)*(numChars + 1))) ){
    perror("malloc");
    exit(EXIT_FAILURE);
  }  

  /* reads yeash chariter in the document */
  lseek(infile, 0, SEEK_SET);
  if( (in = read(infile, Buffer, (sizeof(char)*(numChars + 1)))) > 0){        
    
    for(i = 0; i < numChars; i++){     
      /* for each chariter in the document find its code */
      c = Buffer[i];
     
      code =  codes[(unsigned int) c ];
      poseInCode = 0;
    
      /* printf("new code %s\n", code);  */ 

      /*figure out how to right variable lenght codes to a bit */
      while( poseInCode < strlen(code) ){ 
           
             
      
         if(code[poseInCode++] == '1'){
  	   buff |= 1<< ((X8_BIT-1) - poseInBuff);           
         }
    
     
       
         if( poseInBuff++ > (X8_BIT - 2) ){
            
	    CodeBuff[countBuff++] = buff;
            /* write(outfile, &buff, sizeof(buff)); */
            poseInBuff = 0;
            buff = 0;
         } 
       
       }     
   } 
 
  }  
  if(buff){
      CodeBuff[countBuff++] = buff;
      
  }
 
  if(countBuff) 
    write(outfile, CodeBuff, sizeof(char) *(countBuff)); 
  free(Buffer);
  free(CodeBuff);
}
