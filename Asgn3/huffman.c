#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdint.h>

#include "huffman.h"

#define START_POS 2


typedef struct list {
  int size;
  node **entrys;
} list;

typedef int (*comp) (const void *, const void *); 

void mCode(node *head, char *codes[], int depth, char code[]){
/*recursivle moves doen a tree to gennerate a hufman code */
  int nextDepth;
  char *spot;
  int i;  
  
  nextDepth = depth+1;


  if(head){
    if(head->left == NULL && head->right == NULL){
      /* make room for the code and copy it over to the list of codes */
      if( !(codes[head->c] = (char*) malloc(nextDepth*sizeof(char))) ){
	perror("malloc");
	exit(EXIT_FAILURE);
      } 
      
      code[depth] = '\0';
      spot = codes[head->c];
      /* copys every thing in code string to the codes array 
         at the index determend by the chariter at the leaf
      */ 
      for(i = 0; i < nextDepth; i++){
         spot[i] = code[i];
      }
    
     } else {
     
      /* add the apropreat chariter to the string and make the
         recursive calls
      */ 
     code[depth] = '0';
     mCode(head->left, codes, nextDepth , code);
     code[depth] = '1';
     mCode(head->right, codes, nextDepth, code); 
     
    }
  }

}

void make_code(list *List, char *codes[]){
/* intialises the variables needed for the recursive call to mcode */ 
  char code[NUM_CHAR+1];
  
  mCode(List->entrys[0], codes, 0, code);

}

list *insert_node(list *List, node *newNode){
  /* makes a new smaller list with the fist two spots removed
     and inserts the new node into the firts position were the next
     node freq is => its own
  */ 

  list *newlist;
  node *current;
  int pos, k, size, flag;
  uint32_t freq;
  freq = newNode->freq;
  size = List->size - 1;
  
  /*initalizes the new list*/
  if( !(newlist = malloc(sizeof(list))) ){
    perror("malloc");
    exit(EXIT_FAILURE);
  }

  if( !(newlist->entrys = malloc(sizeof(node*)*(size))) ){
    perror("malloc");
    exit(EXIT_FAILURE);
  }
  
  /*make the new list without the first two places in the original
    list and insirt thr new node into the proper location
  */
  for(pos = START_POS, k = 0, flag = 0; pos < size + 1; k++){
    current = List->entrys[pos];
    if (flag == 0 && freq <= current->freq){
      newlist->entrys[k] = newNode;
      flag = 1;
      continue; 
    }
    newlist->entrys[k] = current;
    pos++; 
   
  }
  /* if the ne node had not be put into the new list put it on the end */
  if (flag == 0){
      newlist->entrys[k] = newNode;
      flag = 1;
   
  }
   
  newlist->size = size;
  free(List->entrys);
  free(List);
  
  return newlist;
  


}

node *make_huffman_node(node *left, node *right){
  /*makes a new node with left and right broanches 
    that are the nodes in the arguments and whos 
    freq is the combination of the two 
  */
  node *new;
 
  if( !(new = malloc(sizeof(node))) ){
     perror("malloc");
     exit(EXIT_FAILURE);

  }
  new->c = '\0';    
  new->freq = (right->freq + left->freq);
  new->right = right;
  new->left = left;
  

  return new;
  
}


int comp_list(node **n1, node **n2){
  /*takes in two pointers to node pointers and arrages 
    them by lest frequent of lowist char value
  */

  int diff;
  diff =  ((*n1)->freq - (*n2)->freq);
  if(diff)
    return diff;
  else
    return  ((*n1)->c - (*n2)->c); 
   
  
}

list *sort_list(list *sList){
  /* sort the list based on the rulls from qsort */
  node **entrys;
  entrys = sList->entrys;
  
  qsort(entrys, sList->size, sizeof(node*), (comp) comp_list);

  return sList; 
}
 
int* char_freq(int file){
  /* takes a file and reads each charicter in that file
     for each charter the askii value of that charicter is 
     used as the idex of an array and its value is the nuber 
     is the frequence of the array
  */
  
  int *freq, in;
  unsigned char c;

  
  
  if(-1 == file){
    perror("file");
    exit(EXIT_FAILURE);
  }
  
  /*initalis the freq array to 0 */
  if( !(freq = (int*)calloc(NUM_CHAR, sizeof(int))) ){
      perror("calloc");
      exit(EXIT_FAILURE);
    }

  
  
   while( (in = read(file, &c, sizeof(char))) > 0){  
    
    freq[(int)c]++;     

  }
  return freq;
}

list *make_list(int *freq){
  /*makes a list of node out of the int array bassed into it
    each nodes c is based on the index of in the freq array
  */
  
  node *entry;
  list *sList;
  int count, i; 

  count = 0;
  /*makes room for the list and sets its anital size to 0 */
  if( !(sList = malloc(sizeof(list))) ){
    perror("malloc");
    exit(EXIT_FAILURE);
  }

  if( !(sList->entrys = malloc(sizeof(node*)*(NUM_CHAR))) ){
    perror("malloc");
    exit(EXIT_FAILURE);
  }
  
  sList->size = 0;
  /* for each spot on the freq array that is not 0 make a new node */
  for(i = 0; i < NUM_CHAR; i++){
    if(freq[i] != 0){

      /*make new node and add it to the list */

      if( !(entry = malloc(sizeof(node))) ){
	perror("malloc");
	exit(EXIT_FAILURE);

      }
      entry->c = i;
      entry->freq = freq[i];
      entry->right = NULL;
      entry->left = NULL;
      
      sList->entrys[count++] = entry;

    }
  }
  
  sList->size = (count);

  /* to avoud a realloc of zero */   
  if( !count ){
    return sList;
  }
  
  if( !(sList->entrys = realloc(sList->entrys,sizeof(node*)*count)) ){
    perror("realloc");
    exit(EXIT_FAILURE);
  }
  return sList;



}

void free_tree(node *head){
  /*recusivly frees the tree*/
  node *left, *right;
  
  if(head){
    
    left = head->left;
    right = head->right;
    free(head);

    free_tree(left);
    free_tree(right);  
      
  }
  

}

void free_codes(char *codes[]){
  /* frees the elaments of the codes array */
  int i;
  for( i = 0; i < NUM_CHAR; i++){
    if(codes[i])
      free(codes[i]);
  }


}

void free_list(list *List){
  /* frees everyting on the list including trees*/

  int i;
  for(i = 0; i < List->size; i++){
     free_tree(List->entrys[i]);
         
   
 }
  free(List->entrys);
  free(List);
 

}



int make_htable(int *freq, char *codes[]) {
  
  list *List;
  node *entry;
  int i, numChars;

  
  /* enitalize the table */

  for( i = 0; i < NUM_CHAR; i++){
    codes[i] = NULL;
  }
  
  
  List = make_list(freq);
  List = sort_list(List);
 
  /*if there is nothing on the list free and retutn */
  if(List->size == 0){
      
      free_list(List);
      return 0;
  }
  /* as long as there are 2 or more elaments on a list make a new node 
     and reinsert it into the list 
  */ 
   while (List->size > 1){
    entry = make_huffman_node(List->entrys[0], List->entrys[1]);
    List = insert_node(List, entry);
  
   }
 
  numChars = List->entrys[0]->freq;
  make_code(List, codes);
      
  free_list(List);
  return numChars;
}

node *make_tree(int *freq){
  list *List;
  node *entry, *tree;

  List = make_list(freq);
  List = sort_list(List);

   /*if there is nothing on the list free and retutn */
  if(List->size == 0){
      
      free_list(List);
      return NULL;
  }
  /* as long as there are 2 or more elaments on a list make a new node 
     and reinsert it into the list 
  */ 
   while (List->size > 1){
    entry = make_huffman_node(List->entrys[0], List->entrys[1]);
    List = insert_node(List, entry);
  
   }
   tree = List->entrys[0];    
   
   free(List->entrys);
   free(List);
   
   return tree;
   

}
