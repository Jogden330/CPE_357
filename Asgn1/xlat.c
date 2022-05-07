#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void translate(char set1[], char set2[]);
void delete(char set1[]);
void complement(char set1[]);

int main(int argc, char* argv[]){
  /* 
  Selects the function based on the camand line arguments.
  If not an agument shows a useage message and exits the program.
  */

  if(argc == 4 && !strcmp(argv[1],"-d") && !strcmp(argv[2],"-c")) { 
    
    complement(argv[3]);

  } else if((argc == 3)&&(!strcmp(argv[1],"-d"))){

    delete(argv[2]);

  } else if(argc == 3 && strcmp(argv[2],"")){

    translate(argv[1], argv[2]);

  } else {
    fprintf(stderr, "usage: %s [-d] [-c] set1 [set2]\n", argv[0]);
    /* if entered incorectly the usage is shown*/
    exit(EXIT_FAILURE);
  }
  
  return 0;
}

void translate(char set1[], char set2[]){
  /* 
  replaces the charicer in that match set1 with 
  the charicter in set2 at the same position
  from standered input, then right that to standered output. 
  */


  int i, j, flag; /* index to check */
  char replacment;
  
  /* takes in standered input */
  while((i = getchar()) != EOF){
    

    flag = 0;
    for(j = 0; j < strlen(set1); j++){  
      if(i == (unsigned char) set1[j]){
	
	flag = 1; /* flag if mack */
	  if( j < strlen(set2)){
	    /* if set1 has repeated letters set the last one for replacment  */
	     replacment = set2[j];
             
	    } else {
	    /* if set1 is longer then set2 use the last letter of set2 */
	    replacment = set2[(strlen(set2)-1)];
	    
	      
	    }
	}
 
    }
    /* if a mach is found replace the letter otherwise print the original */
    if(flag == 0){       
	putchar(i); 
      } else {
	putchar(replacment);
      }
  }


}

void delete(char set1[]){
  /* 
   removes all charicters in standered input that appeare in set1,
   then rights to stadered output.
  */
 
  int i, j, flag = 0;
  /* takes in standered input */
  while((i=getchar()) != EOF){ 
  
    flag = 0;
    
    /* iterates through set1 */
    for(j = 0; j < strlen(set1); j++){
        
      if(i == (unsigned char) set1[j]){
	/* 
	   if there is a match in set1 set a flag
	   and break out of for loop.
	*/ 
	  flag = 1;
	  break;
       }
    
    }
    if(flag == 0){
      /* if the was no mach right letter to standered output */
	putchar(i);
      }

  }

}

void complement(char set1[]){
  /* 
  removes all charicters in standered input that dont appeare in set1,
  then rights to stadered output.
  */
  
    int i, j, flag = 0;
 
    while((i=getchar()) != EOF){ /* get statandered input */ 
   
    flag = 0;

    for(j = 0; j < strlen(set1); j++){ /* iterate through set1 */
      
      if(i == (unsigned char) set1[j]){ /* if the is a mach set flag */
	  flag = 1;
	  break;
       }
    
    }
    if(flag != 0){ /* if the is a mach right charicter */ 
	putchar(i);
      }

  }

}

