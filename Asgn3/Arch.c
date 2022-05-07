
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>

#define NAME_OFFCET 0
#define NAME_LENGTH 100
#define UID_OFFCET 108
#define UID_LENGTH 8
#define GID_OFFCET 116
#define GID_LENGTH 8
#define SIZE_OFFCET 124
#define SIZE_LENGTH 12
#define MTIME_OFFCET 136
#define MTIME_LENGTH 12
#define CHKSUM_OFFCET 148
#define CHKSUM_LENGTH 8
#define TYPEFLAG_OFFCET 156
#define TYPEFLAG_LENGTH 1
#define LINKNAME_OFFCET 156
#define LINKNAME_LENGTH 100
#define MAGIC_OFFCET 257
#define MAGIC_LENGTH 6
#define VERSION_OFFCET 263
#define VERSION_LENGTH 2
#define UNAME_OFFCET 265
#define UNAME_LENGTH 32
#define GNAME_OFFCET 297
#define GNAME_LENGTH 32
#define DEVMAJOR_OFFCET 329
#define DEVMAJOR_LENGTH 8
#define DEVMINOR_OFFCET 337
#define DEVMINOR_LENGTH 8
#define PREFIX_OFFCET 345
#define PREFIX_LENGTH 155
#define BLOCK_SIZE 512

void make_header(int infile, int outfile){

  

}

int archive_creator(int infile, int outfile){
  /*takes an infinle to arcive and  archives it
    then rights that to an outfile retutns zero 
    if nothing whent wrond and non zero if there 
    was an error 
  */
  
  make_header(infile,outfile);


}
