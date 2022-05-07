#include<signal.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/time.h>
#include <signal.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/time.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#define HALF_SECOND 500000

int count = 0; 
unsigned long numeTicks = 0;

void handler(int sig) { 

  if( !(numeTicks++ % 2) ){
            
    printf("Tick..");
      
  } else {

    printf("Tock\n");    
    count++;
    

  }
  fflush(stdout);
}

void usage(char *name) {
  fprintf(stderr,"usage: %s +nume>0\n",name);
  exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {

  struct sigaction singnal;
  struct itimerval timer;
  sigset_t mask,old;
  char *end;
  int i, endtime;

  
  if ( argc!=2)
     usage(argv[0]); 

  if  ((endtime = strtol(argv[1],&end,10)) <= 0) 
     usage(argv[0]);

  if( *end )
    usage(argv[0]);

 
  sigemptyset(&mask);
  sigaddset(&mask,SIGALRM);

  if ( -1 == sigprocmask(SIG_BLOCK, &mask, &old) ) {
    perror("sigprocmask");
    exit(EXIT_FAILURE);
  }

  singnal.sa_handler = handler;
  sigemptyset(&singnal.sa_mask);
  singnal.sa_flags = 0;

  if ( -1 == sigaction(SIGALRM, &singnal, NULL) ) {
    perror("sigaction");
    exit(EXIT_FAILURE);
  }

  timer.it_interval.tv_sec = 0;
  timer.it_interval.tv_usec =  HALF_SECOND;
  timer.it_value = timer.it_interval;

  if ( -1 == setitimer(ITIMER_REAL, &timer, NULL) ) {
    perror("setitimer");
    exit(EXIT_FAILURE);
  }

  while( count <= (endtime - 1)) {
    sigsuspend(&old);    
  }
  printf("TIme's up!\n");

  return 0;
}
