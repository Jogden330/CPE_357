#include <stdio.h>
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

#define PARRENT ".."
#define THISDIR "."
#define ROOT "/"



char *pwd(char *currentPath,char *parrentPath, char *forwardpath){
  
  DIR *dir;
  char cPath[PATH_MAX+1];
  char pPath[PATH_MAX+1];
  char dPath[2*(PATH_MAX+1)];
  char fPath[PATH_MAX+1];
  
  char *buffer;
  struct dirent *ent;
  struct stat parrent, current;
  
  if( (-1 == stat(currentPath, &current)) ){
    perror(currentPath);
    exit(EXIT_FAILURE);
  
  }    
   if( (-1 == stat(parrentPath, &parrent)) ){
    perror(currentPath);
    exit(EXIT_FAILURE);
  
  }

  /* if the current dir is also root return path */
  if ( (parrent.st_dev == current.st_dev) && (parrent.st_ino == current.st_ino) ) {
      /* printf("path: %s parrent: %ju  current: %ju\n",forwardpath, (uintmax_t)  root.st_ino, current.st_ino ); */
      snprintf(fPath,(PATH_MAX+1),"/%s", forwardpath);
      if( !(buffer = (char *)malloc((PATH_MAX+1)*sizeof(char))) ){
	perror("malloc");
	exit(EXIT_FAILURE);
      } 
      strcpy(buffer, fPath); 
      return buffer;
  }
  
  /* open the directory to the next parrent */
  dir = opendir(parrentPath);

  if ( ! dir ) {
    perror(parrentPath);
    exit(EXIT_FAILURE);
  }
  
  /* check if each entry in the parent directory is eqaual to the current dir */ 
  while ( (ent = readdir(dir)) ) {
    /* stat each entry in the parnet director */
    snprintf(dPath, (2*(PATH_MAX+1)) ,"%s/%s",  parrentPath, ent->d_name);
      
    if( (-1 == stat(dPath, &parrent)) ){
      perror(dPath);
      
    } else {
        /* printf("path: %s parrent: %ju  current: %ju\n",dPath, (uintmax_t)  parrent.st_ino, current.st_ino );  */

      if ( (S_ISREG(parrent.st_mode) || S_ISDIR(parrent.st_mode)) ){ 
	/* check if there i nod numbers and devices are the same */
          if ( (current.st_dev == parrent.st_dev) && (current.st_ino == parrent.st_ino) ){

	    /* add the path name of the dir to the path name and do it again */
	    snprintf(cPath, (PATH_MAX+1),"%s/%s", PARRENT, currentPath); 
	    snprintf(pPath, (PATH_MAX+1),"%s/%s", PARRENT, parrentPath);
            
            /* if the forwaord path is empty do not put a / in front of d_name */
            if( strlen(forwardpath)){
	       snprintf(fPath,(PATH_MAX+1),"%s/%s", ent->d_name, forwardpath);
            } else {
               snprintf(fPath,(PATH_MAX+1),"%s", ent->d_name);

            }
            /* if the paths are full print error and exit */
            if( !(strlen(cPath) < PATH_MAX) || !(strlen(pPath) < PATH_MAX) || !(strlen(fPath) < PATH_MAX) ){
                 closedir(dir); 
                 printf("path too long\n");
                 exit(EXIT_FAILURE);
            }
            closedir(dir);
            return pwd(cPath, pPath, fPath);
          }
       } 
    }
  }
  printf("cannot get current directory.\n");
  exit(EXIT_FAILURE);  
  
} 

int main(){
  char *path, *fpath;
  fpath = "";
  

  path = pwd(THISDIR, PARRENT, fpath);

  if( (strlen(path) < PATH_MAX) ){
    printf("%s\n", path);
  } else {
    printf("path too long\n");
  }
  free(path);
  return 0;

}
