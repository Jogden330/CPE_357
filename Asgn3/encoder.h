#ifndef ENCODERH
#define ENCODERH

int write_header(int outfile, int *freq);
void encode_file(int infile, int outfile, char *codes[], int numChars);

#endif
