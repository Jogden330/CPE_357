#ifndef HASHH
#define HASHH

typedef struct node node;

typedef struct table table;  

unsigned long hash(char *key, int size);

table *make_table();

void insert(char *word,table *hashtable);

void print_n_table(table *hashtable, int n);

void free_table(table *hashtable);

void rehash(table *hashtable);

#endif
