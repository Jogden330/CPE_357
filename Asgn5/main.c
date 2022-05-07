#include <arpa/inet.h>
#include <getopt.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include "chat.h"

#define MIN_ARGS 2 

void usage(char *name){

  printf("%s [-v] [-a] [-N] [hostname] port\n", name);
  exit(EXIT_FAILURE); 

}


int main(int argc, char *argv[]) {
  int port, v, a, N;
  char *hostname, *end;
  char flag;
  v = 0;
  a = 0;
  N = 0;
  hostname = NULL;
  

  if ( MIN_ARGS  > argc )
     usage(argv[0]);

  while ( (flag = getopt(argc, argv, "vaN")) !=EOF  ){

    switch(flag){

        case 'v':
	  v++;
          break;
        case 'a':
          a++;
	  break;
        case 'N':
          N++;
	  break;
        default:
	  usage(argv[0]);

    } 
  }
 

  if ( (MIN_ARGS-1)  > (argc - optind) )
     usage(argv[0]);

  if((optind + 2) == argc){ 
   
    hostname = argv[optind++];
  }

  port = strtol(argv[optind++],&end,10);
  
  if (*end)
      usage(argv[0]);
    

  if (!(hostname)){
    
    chat_server(port, a, v, N);      
  } else {
    
    clinent_chat(hostname, port, a, v, N); 
  } 

  return 0;
}
