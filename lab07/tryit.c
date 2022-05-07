#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void usage(char *name){
  
  printf("usage: %s command\n", name);
  exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
  pid_t child;
  int status;
  
  
  if (argc != 2)
    usage(argv[0]);

  
  if ( (child=fork())) {
    /* parent or error */
    if ( child == -1 ) {
      perror("fork");
      exit(EXIT_FAILURE);
    }
    

    if ( -1 == wait(&status) ) {
      perror("wait");
      exit(EXIT_FAILURE);
    
    }
    
    if ( ! WIFEXITED(status) || WEXITSTATUS(status) ){
      printf("prosses %d, exit with an error value.\n", getpid());
      exit(EXIT_FAILURE);
    }
    printf("prosses %d, succseeded.\n", getpid());

  } else {
    /* child */
      execl(argv[1], argv[1], (char*) NULL);
    perror(argv[1]);
    exit(EXIT_FAILURE);
  }


  return 0;
}
