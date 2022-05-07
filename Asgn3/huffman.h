#ifndef HUFFMANH
#define HUFFMANH

#include<stdint.h> 
#define NUM_CHAR 256

  
typedef struct node {
     unsigned char c;
     uint32_t freq;
     struct node *left;
     struct node *right;
 } node;

 
int make_htable (int *freq, char *codes[]);
void free_codes (char *codes[]);
int *char_freq(int file);
node *make_tree(int *freq);
void free_tree(node *head);

#endif
