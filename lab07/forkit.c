#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


int main() {
  pid_t child;
  int status;
  

  printf("Hellp, World!\n");
  if ( (child=fork())) {
    /* parent or error */
    if ( child == -1 ) {
      perror("fork");
      exit(EXIT_FAILURE);
    }
    printf("This is the parent, pid %d.\n", getpid());

    if ( -1 == wait(&status) ) {
      perror("wait");
      exit(EXIT_FAILURE);
    
    }
    printf("This is the parent, pid %d, signing off.\n", getpid());

    if ( ! WIFEXITED(status) || WEXITSTATUS(status) )
      exit(EXIT_FAILURE);

  } else {
    /* child */
    printf("This is the child, pid %d.\n", getpid());
  }


  return 0;
}
