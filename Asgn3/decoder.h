#ifndef DECODER_H
#define DECODER_H

#include "huffman.h"

int* get_header(int infile);
void decode_file(int infile, int outfile, struct node *tree);
void encode_file(int infile, int outfile, char *codes[]);

#endif
