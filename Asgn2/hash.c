#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<ctype.h>

#include "hash.h"

#define START_SIZE 1000
#define HASH_CODE 43
#define DELTA 1.5
#define MAKE_BIGGER 10
#define START_COUNT 1

/* define the nodes that make up the table */
typedef struct node {
  unsigned long key;
  char *word;
  int count;
  struct node *next;
  struct node *prev;
} node;

//typedef struct node *node;

typedef struct table {
  int size;
  int numEntrys;
  node **entry;
} table;


/* define the table of nodes */


unsigned long hash(char *key, int size){

  /*
  generate a hash and retutn that
  a nuber from 0 to the table size
  */

  unsigned long code;
  int i;

  for( i = 0, code = 0; i < strlen(key); i++){
    code = key[i] + HASH_CODE * code;
  }
  
  return code % size;
  
}

table *build_table(int size){
  /* creats a table of a size determind by the input
     sets each spot on the table to NULL and sets the 
     nuber of entrys to 0
  */

  int i;
  table *hashtable;

  if( !(hashtable = malloc(sizeof(table))) ){
	perror("malloc");
	exit(EXIT_FAILURE);
  }

  if( !(hashtable->entry = malloc(sizeof(node*)*(size))) ){
	perror("malloc");
	exit(EXIT_FAILURE);
  }
  
      /* enitalize the table */
    
  for( i = 0; i < size; i++){
    hashtable->entry[i] = NULL;
  }
  hashtable->size = size;
  hashtable->numEntrys = 0;
    
  return hashtable;
  
}

table *make_table(){
  /* makes a table of the starting size */
  return build_table(START_SIZE);

}

void insert_node(char *word,table *hashtable, int count, unsigned long index){
  /* adds a node to the first index of a linked list on a hashtable
     whith a givin count
  */
  node *entry;
  
  /* finds the corisponding index on the tablle */
 
      
  node *spot = hashtable->entry[index];
  
  
  /* creat the new node */
  if( !(entry = malloc(sizeof(node))) ){
	perror("malloc");
	exit(EXIT_FAILURE);
  
  }  
 
  /*set node and add to hashtable at the begining of the linked list */  
  entry->word = word;
  entry->count = count;
  entry->next = NULL;
  entry->prev = NULL;

  if(hashtable->entry[index] != NULL){
    hashtable->entry[index]->prev = entry;
    
  } 
  entry->next = hashtable->entry[index];
  entry->key = index;

  hashtable->entry[index] = entry;
  hashtable->numEntrys++;
	
}


void insert(char *word,table *hashtable){
  /* adds a node to the first index of a linked list on a hashtable
     if a node with the same word allready exists increas that nodes 
     count 
  */
  node *entry;
  
  /* makes the hash value and finds the corisponding index on the tablle */
  unsigned long index = hash(word, hashtable->size);
 
     
  node *spot = hashtable->entry[index];
   
  /* if the word is already on the table increas its count and return */
  while(spot != NULL){
    if( !strcmp(spot->word,word)){
      spot->count++;
      free(word);
      return;
    }
    spot = spot->next;
  }
  /* if it was not allready on the table make a new node */ 
  insert_node( word, hashtable, START_COUNT, index);
  	
}


void insert_wCount(char *word,table *hashtable, int count){
  
  /* makes the hash value and finds the corisponding index on the table
     this is a glorified overloded function  
  */
  unsigned long index = hash(word, hashtable->size);

  insert_node(word, hashtable, count, index);	
}

void  print_n_table(table *hashtable, int n){
  /* serch the table N times for the node with the largist count 
     if to have the same count choose the lagist string
     then remove the node and print its infor to stdout
  */
  int size, count, i;
  node *spot, *big, *tmp;
  unsigned long index;
  size = hashtable->size;

  count = 0;
  big = NULL;
  printf("The top %d words (out of %d) are:\n",n , hashtable->numEntrys);
  
  /* search the table h n times */
  while(count < n){
    
  
    for(i = 0; i < size; i++){
     spot = hashtable->entry[i];
      
      if(spot != NULL){
	
	while(spot != NULL){
	  if( (big == NULL) || (big->count < spot->count) ||
	      ( (big->count == spot->count) &&
		(strcmp(big->word,spot->word) < 0) )){
	              
	            big = spot;
	  }
	  spot = spot->next;
	}
        
      }
    }
    /* if nothing was found dint print anything and return */
  if(big == NULL)
     return;
  count++;
  index = big->key;  
  printf("%9d %s\n",big->count, big->word);
  
  /* remove the node from the list */
  if(hashtable->entry[index] == big){
     hashtable->entry[index] = big->next;
  }

  if(big->next != NULL){
    big->next->prev = big->prev; 
  }
  
  if(big->prev != NULL){
    big->prev->next = big->next;
   
  }
  /* free the removed node */
  free(big->word);
  free(big);
  big = NULL;  
  
  }
  

}

void free_table(table *hashtable){
  /* free all the nodes on the table and then free the table */
  int size, i;
  node *spot, *tmp;
  size = hashtable->size;
  /* look for each node on the table and free them */
  for(i = 0; i < size; i++){
     spot = hashtable->entry[i];
    
     while(spot != NULL){
      
       free(spot->word);
       tmp = spot;
       spot = spot->next;
       free(tmp);     
      }

      free(spot);
  
  }
  free(hashtable->entry);
  free(hashtable);


}

void rehash(table *hashtable){
  
  table *new, *old;
  //unsigned long index;  
  int newSize, count, i;
  node *tmp, *spot;
  char *word;

  /* check if sould be rehashed */
  if(! ((hashtable->numEntrys) > (DELTA*hashtable->size)) )         
    return;

  old = hashtable;
  newSize = MAKE_BIGGER*(old->size);
  /*makes a new hashtable of a biger size */
  new = build_table(newSize);

  /* chreats new nodes for each node on the old table and 
     adds them to the new one with there counts
     and frees each space
  */
  for(i = 0; i < old->size; i++){
      spot = old->entry[i];

	while(spot != NULL){
	    
	  tmp = spot;
	  word = spot->word;
          count = spot->count;
          insert_wCount(word, new, count);
	  spot = spot->next;
	  free(tmp);
	} 
	free(spot);
    }
  /* set the hashtable to the new one and free the space */ 
  free(hashtable->entry);
  *hashtable =  *new;
  free(new);

}
