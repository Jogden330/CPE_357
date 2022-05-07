#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/stat.h>  
#include <fcntl.h>
#include <dirent.h>
#include <inttypes.h>
#include <limits.h>
#include <stdio.h>
#include <sys/types.h>
#include <limits.h>
#include <stdint.h>  
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <utime.h>

#include "Arch.h"
#define PERM_LEN 4
#define OG_LEN 17
#define Y_M_D_LEN 10
#define TIME_LEN 5
#define FULL_PATH_LEN 255

static int insert_special_int(char *where, size_t size, int32_t val) {
  /* For interoperability with GNU tar.  GNU seems to
   * set the high-order bit of the first byte, then
   * treat the rest of the field as a binary integer
   * in network byte order.
   * Insert the given integer into the given field
   * using this technique.  Returns 0 on success, nonzero
   * otherwise
   */
  int err=0;

  if ( val < 0 || ( size < sizeof(val))  ) {
    /* if it's negative, bit 31 is set and we can't use the flag
     * if len is too small, we can't write it.  Either way, we're
     * done.
     */
    err++;
  } else {
    /* game on....*/
    memset(where, 0, size);     /*   Clear out the buffer  */
    *(int32_t *)(where+size-sizeof(val)) = htonl(val); 
    /* place the int */
    *where |= 0x80;             /* set that high-order bit */
  }

  return err;
}  

void perm_gen(char *perm, int mode){

  int i;
  int mask;  

  for(mask = PERM_LEN, i=0; i < PERM_LEN - 1; i++){
    if( !(mode & mask) )
      perm[i] = '-'; 
      
    mask >>= 1;
  }
 

}

void print_header(struct header *head){
  
  char type;
  char owner[PERM_LEN];
  char group[PERM_LEN];
  char other[PERM_LEN];
  char OG[OG_LEN];
  char yearMonthDay[Y_M_D_LEN];
  char hourMinut[TIME_LEN];
  char pathName[FULL_PATH_LEN];
  int size;
  time_t mt;
  struct tm *theTimes;
 
  strcpy(owner, "rwx");
  strcpy(group, "rwx");
  strcpy(other, "rwx");
  
  perm_gen(owner, (int) head->mode[4]); 
  perm_gen(group, (int) head->mode[5]);
  perm_gen(other, (int) head->mode[6]); 

  switch(head->typeflag[0]){
     case '5':
       type = 'd';
       break;
     case '2':
       type = 'l';
       break;
     case '0':
       type = '-';
       break;
     case '\0':
       type = '-';
       break;
     default:
       printf("invalide typeflag\n");
       exit(EXIT_FAILURE);  
  }
    
  snprintf(OG, 17,"%s/%s", head->uname, head->gname );
  size = strtol(head->size, NULL, 8);
  mt = strtol(head->mtime, NULL, 8);

  theTimes  = localtime(&mt);  
  strftime(yearMonthDay, (Y_M_D_LEN + 1), "%Y-%m-%d", theTimes);       
  strftime(hourMinut, (TIME_LEN + 1), "%H:%M", theTimes);

  if(head->prefix[0]){
             snprintf(pathName, (FULL_PATH_LEN + 1), "%s/%s",
	     head->prefix,
             head->name);
  } else {
    snprintf(pathName, (NAME_LENGTH + 1), "%s", head->name);
  }         
 
  
  printf("%c%s%s%s %.17s %8d %s %s %.255s\n",
         type, owner, group, other, OG, size,
         yearMonthDay, hourMinut, pathName );  

  
}

int check_haed(header *head){
  header *dummy;
  int error, sum, i;
  char ver;
  char tempChksum[CHKSUM_LENGTH];
  char magic[MAGIC_LENGTH];
  ver = '0';
  error = 0;
  dummy = head;

  strcpy(magic, "ustar");
  strcpy(tempChksum, head->chksum);

  memset(head->chksum, ' ', CHKSUM_LENGTH);
  
  for ( sum = 0, i = 0; i < (BLOCK_SIZE); i++){
 
    sum += ((unsigned char *)dummy)[i];
   

  }
  snprintf(head->chksum, CHKSUM_LENGTH, "%07o", sum);
  if ((strcmp(head->chksum, tempChksum)))
    error++; 
  if((strcmp(head->magic, magic)))
    error++;
  if((head->version[0] != ver))
    error++;
  if((head->version[1] != ver))
    error++;
    
  return error;
}

header *make_header(struct stat toTar, char *path, int S){
  int i, rlink;
  unsigned int sum;
  char name[PATH_MAX+1];
  char linkname[LINKNAME_LENGTH];
  struct header  *head, *dummy;
  struct group *grp;
  struct passwd *pwd;

  strcpy(name,path);

  if( !(head = calloc(1,sizeof(header))) ){
    perror("calloc");
    exit(EXIT_FAILURE);

  }  
  
  
  if(strlen(name) < NAME_LENGTH){
  
    snprintf(head->name, NAME_LENGTH + 1, "%s" ,name);
   
  } else {
    for ( i = ((NAME_LENGTH + 1) - strlen(name));
          i < strlen(name); i++){ 
      if(name[i] == '/')
	 break;

    }
      snprintf(head->prefix, i, "%s" ,name);
      snprintf(head->name, NAME_LENGTH + 1, "%s" ,(name + i));
 
  }

  snprintf(head->mode, MODE_LENGTH+1, "%07o",
          (unsigned int) (toTar.st_mode &
          (S_IRWXU | S_IRWXG | S_IRWXO)));
 
  if(S) { 
      snprintf(head->uid, UID_LENGTH, "%07o" ,
              (unsigned int) toTar.st_uid);
  } else {
      insert_special_int(head->uid, UID_LENGTH, toTar.st_uid);
  }
  snprintf(head->gid, GID_LENGTH, "%07o" , 
          (unsigned int) toTar.st_gid);
  /* if toTar is a derectoy or a symlink set size to zero 
     if not set the size */
  
  snprintf(head->mtime , MTIME_LENGTH, "%011o" , toTar.st_mtim.tv_sec);


  memset(head->chksum, ' ', CHKSUM_LENGTH);


  if (S_ISDIR(toTar.st_mode)){
    snprintf(head->size, SIZE_LENGTH, "%011o" , 0);
    head->typeflag[0] ='5';
   
  } else if (S_ISLNK(toTar.st_mode)) {
    snprintf(head->size, SIZE_LENGTH, "%011o" , 0);
    head->typeflag[0] = '2';

  } else if (S_ISREG(toTar.st_mode)){
    
    snprintf(head->size, SIZE_LENGTH, "%011o", (unsigned int) toTar.st_size);
    head->typeflag[0] = '0';

  }

  if((S_ISLNK(toTar.st_mode))){
    rlink = readlink(path, linkname, toTar.st_size + 1);

    if (rlink < 0) {
      perror("lstat");
      exit(EXIT_FAILURE);
    }
  
    snprintf(head->linkname, LINKNAME_LENGTH + 1,
             "%s" , linkname);
   
  } else {

     snprintf(head->linkname, LINKNAME_LENGTH, "%s" , "");

  }  

  snprintf(head->magic, MAGIC_LENGTH+1, "%s" , "ustar");
     
  snprintf(head->version, VERSION_LENGTH + 1, "%s" , "00");
 
  pwd = getpwuid(toTar.st_uid);

  snprintf(head->uname, UNAME_LENGTH+1, "%s", pwd->pw_name);
  
  grp = getgrgid(toTar.st_gid); 
  snprintf(head->gname, GNAME_LENGTH+1, "%s" ,  grp->gr_name);


  /* add up all the things in the header */
  dummy = head;
  for ( sum = 0, i = 0; i < (BLOCK_SIZE); i++){
 
    sum += ((unsigned char *)dummy)[i];
  
  }
  
  snprintf(head->chksum, CHKSUM_LENGTH, "%07o", sum);  

  return head;

}

int add_file(int infile, int tarfile, struct header *head){
  
  int numBlocks, size;
  char *buff;
  
  size = strtol(head->size, NULL, 8);
  
  numBlocks = size / BLOCK_SIZE;
   
  if( size % BLOCK_SIZE ) 
    numBlocks++;
  
  if( !(buff = (char *)malloc(sizeof(char)*(numBlocks*BLOCK_SIZE))) ){
    perror("malloc");
    exit(EXIT_FAILURE);
  }  

  read(infile, buff, (numBlocks*BLOCK_SIZE));
  write(tarfile, buff, (numBlocks*BLOCK_SIZE));
  
  free(buff);

  return 0;
}  

int pad_ending(int tarfile){
  char *buff;

  if( !(buff = (char *)malloc(sizeof(char)*(2*BLOCK_SIZE))) ){
    perror("malloc");
    exit(EXIT_FAILURE);
  }
  memset(buff, 0, (2*BLOCK_SIZE));
 
  write(tarfile, buff, (2*BLOCK_SIZE));

  free(buff);

  return 0;

}

void right_path(struct header *head){
  char pathName[FULL_PATH_LEN];

  if(head->prefix[0]){
             snprintf(pathName, (FULL_PATH_LEN + 1), "%s/%s",
	              head->prefix,
                      head->name);
  } else {
             snprintf(pathName, (NAME_LENGTH + 1), "%s", head->name);
  }
  printf("%.255s\n", pathName);

}

int archive_creator(int infile, int outfile, char *path,int S, int v) {
  /*takes an infinle to arcive and  archives it
    then rights that to an outfile retutns zero 
    if nothing whent wrond and non zero if there 
    was an error 
  */
   
   struct stat sb;
   struct  header *head;
      
   if (lstat(path, &sb) == -1) {
     perror("lstat");
     exit(EXIT_FAILURE);
   }
  
   head = make_header(sb, path, S);

   if(v){   
       right_path(head);
   }
   write(outfile, head, sizeof(header));
   add_file(infile, outfile, head);
   free(head);

   return 0;
}

int check_empty(header *head){
  /* returns zero if the header is emptry, non 
     non zero other wise
  */
  int sum, i;
  header *dummy;
  dummy = head;

  for ( sum = 0, i = 0; i < (BLOCK_SIZE); i++){
 
    sum += ((unsigned char *)dummy)[i];
   

  }
  return sum;
}

void make_file(header *head, char *pathName, int perm, int tarfile){
  struct utimbuf mtime;
  int file, numBlocks, size, time;
  char *buff;

  size = strtol(head->size, NULL, 8);
  time = strtol(head->mtime, NULL, 8);
  numBlocks = size / BLOCK_SIZE;
   
  if( size % BLOCK_SIZE ) 
    numBlocks++;
 
  if (-1 == (file = creat(pathName, perm))) {
     perror(pathName);
     exit(EXIT_FAILURE);
  }


  if( !(buff = (char *)malloc(sizeof(char)*(numBlocks*BLOCK_SIZE))) ){
    perror("malloc");
    exit(EXIT_FAILURE);
  }  
  
  read(tarfile, buff, (numBlocks*BLOCK_SIZE));
  
  write(file, buff, size);
  
  mtime.actime = time;
  mtime.modtime = time;  
  utime(pathName, &mtime);

  free(buff);
  close(file);
  
}

int rebild(header *head, int tarfile){
  char *pathName;
  mode_t perm;

  if( !(pathName = (char *)malloc(sizeof(char)*(FULL_PATH_LEN))) ){
    perror("malloc");
    exit(EXIT_FAILURE);
  }

  if(head->prefix[0]){
             snprintf(pathName, (FULL_PATH_LEN + 1), "%s/%s",
	              head->prefix,
                      head->name);
  } else {
             snprintf(pathName, (NAME_LENGTH + 1), "%s", head->name);
  }
  
  perm = strtol(head->mode, NULL, 8);

  switch(head->typeflag[0]){
     case '5':
       mkdir(pathName, perm); 
       break;
     case '2':
       symlink(head->linkname, pathName);
       break;
     case '0':
       make_file(head, pathName, perm, tarfile);
       break;
     case '\0':
       make_file(head, pathName,  perm, tarfile);
       break;
     default:
       printf("invalide typeflag\n");
       exit(EXIT_FAILURE);  
  }
  free(pathName);
  return 0;
}

void read_to_next(header *head, int tarfile){
  int numBlocks, size;
  char *buff;

  size = strtol(head->size, NULL, 8);

  numBlocks = size / BLOCK_SIZE;
   
  if( size % BLOCK_SIZE ) 
    numBlocks++;
 
  if( !(buff = (char *)malloc(sizeof(char)*(numBlocks*BLOCK_SIZE))) ){
    perror("malloc");
    exit(EXIT_FAILURE);
  }  
   
  read(tarfile, buff, (numBlocks*BLOCK_SIZE));
 
  free(buff);
}

int print_table(int tarfile, int v){
  
  struct  header *head;

  if( !(head = calloc(1,sizeof(header))) ){
    perror("malloc");
    exit(EXIT_FAILURE);
  }
  read(tarfile, head, sizeof(header));
  
  while(check_empty(head)){
          if(check_haed(head)){
                  printf("problem reading header, program fail\n");
                  exit(EXIT_FAILURE);
          }
          
          if(v){
	           print_header(head);
          } else { 
                   right_path(head);
          }
                
          read_to_next(head,tarfile);
          read(tarfile, head, sizeof(header));
  }
  free(head);

  return 0;
}

int print_table_name(int tarfile, char *name, int v){
  char pathName[FULL_PATH_LEN+1];
  struct  header *head;
  
  if( !(head = calloc(1,sizeof(header))) ){
    perror("malloc");
    exit(EXIT_FAILURE);
  }
  read(tarfile, head, sizeof(header));
  
  while(check_empty(head)){
          if(check_haed(head)){
                  printf("problem reading header, program fail\n");
                  exit(EXIT_FAILURE);
          }
          
          if(head->prefix[0]){
                  snprintf(pathName, (FULL_PATH_LEN + 1), "%s/%s",
	                   head->prefix,
                           head->name);
          } else {
                  snprintf(pathName, (NAME_LENGTH + 1), "%s", head->name);
          }
                      
          if(!strcmp(name, pathName)){
                  if(v){
	                   print_header(head);
                  } else { 
                           right_path(head);
                  }
	  }      
          read_to_next(head,tarfile);
          read(tarfile, head, sizeof(header));
  }
  free(head);

  return 0;
}

int archive_get(int tarfile, int v){
  
  struct  header *head;

  if( !(head = calloc(1,sizeof(header))) ){
    perror("malloc");
    exit(EXIT_FAILURE);
  }
     
  read(tarfile, head, sizeof(header));
  
  while(check_empty(head)){
 
          if(check_haed(head)){
                  printf("problem reading header, program fail\n");
                  exit(EXIT_FAILURE);
          }
          if(v){
                  right_path(head);
          }
               
          rebild(head,tarfile);
          read(tarfile, head, sizeof(header));
  }
  free(head);
  
  return 0;
}
