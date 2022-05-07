#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<ctype.h>
#include <arpa/inet.h>

#include "huffman.h"
#include "decoder.h"

#define X8_BIT 8
#define X32_BIT 32

int*  get_header(int infile){
  unsigned int i, numchar;
  uint8_t c;
  uint32_t freqOf;
  int *freq;
 
  numchar = 0;  

  if(-1 == infile){
    perror("infile");
    exit(EXIT_FAILURE);
  }

   if( !(freq = (int*)calloc(NUM_CHAR, sizeof(int))) ){ 
     perror("calloc"); 
    exit(EXIT_FAILURE); 
  } 
 
  
  if ( (read(infile, &c, sizeof(c))) > 0){
     numchar = c + 1;
     
  } else {
    
    return freq;
  }
  
  for (i = 0; i < numchar; i++){
    
    read(infile, &c, sizeof(c));
    read(infile, &freqOf, sizeof(freqOf));
    freqOf = ntohl(freqOf);
    freq[(int)c] = (int) freqOf;       
     

  } 
  return freq;
}

char find_char(unsigned long *place, char *buff, node *tree){
  char c;
  if( (tree->left == NULL) & (tree->right == NULL) )
    return tree->c;
  c = buff[(*place)++];
  
  switch(c){
  case '0' :
       return find_char(place, buff, tree->left);
       break;
  case '1':
     return find_char(place, buff, tree->right);
     break;
  default:
    printf("Probem Durring Tree Taversal");
    exit(3);
    break;
  }
}


void decode_file(int infile, int outfile, node *tree){
  unsigned long buffSize, pose, *place, charAt;
  unsigned int mask, i, in;
  char *buff, *string, c;
  pose = 0;
  charAt = 0;

  if(tree == NULL)
    return;
  buffSize = tree->freq;  

  if( !(buff = (char *)malloc(sizeof(char)*(buffSize + 2)*X8_BIT)) ){
    perror("malloc");
    exit(EXIT_FAILURE);
  } 
 
  if( !(string = (char *)malloc(sizeof(char)*(buffSize + 2))) ){
    perror("malloc");
    exit(EXIT_FAILURE);
  }    

  while( (in = read(infile, &c, sizeof(char))) > 0){
    mask = 0;
    for(mask = 1<<(X8_BIT - 1), i=0; i < X8_BIT; i++){
 
      buff[pose++] = (c & mask) ? ('1') : ('0');
      
        mask >>= 1;
    }

  }

  buff[pose] = '\0';
  pose = 0;
  place = &pose;
  
 

  while(buffSize-- > 0){
    string[charAt++] = (unsigned char)  find_char(place, buff, tree);
        
  } 
  string[charAt] = '\0';
  /* printf("string is %s it is %d long\n", string, charAt); */
  write(outfile, string,sizeof(char)*charAt);
  free(buff);
  free(string);

   
}


