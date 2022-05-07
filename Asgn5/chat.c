#include <arpa/inet.h>
#include <getopt.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <unistd.h>
#include <poll.h>
#include <unistd.h>  
#include <ctype.h>
#include<signal.h>

#include "chat.h"

#define DEFAULT_BACKLOG 100
#define MESG "Hello, TCP Client.\n"
#define MAXLEN 1000
#define CONFERM "ok\0"
#define DECLINE "go away\0"
#define END_CALL "\nConnection closed. ^C to terminate.\0"

void handler(int signum) {
    
}

void chatclient(int socket, int v, int N) {
  
    int  len, mlen;
    char buff[MAXLINE];
    struct pollfd fds[REMOTE+1];
    struct sigaction act;
    sigset_t mask,old;

    fds[LOCAL].fd = STDIN_FILENO;
    fds[LOCAL].events = POLLIN | POLLHUP;
    fds[LOCAL].revents = 0;
    fds[REMOTE] = fds[LOCAL];
    fds[REMOTE].fd = socket;
    strcpy(buff, "");

    if (!N)
        start_windowing();

    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    act.sa_handler = handler;   
    sigemptyset(&mask);
    sigaddset(&mask,SIGINT);

    set_verbosity(v);    
 
    do {

      poll(fds,sizeof(fds)/sizeof(struct pollfd),-1); /* negative->wait forever */
      
      if (fds[REMOTE].revents & POLLIN ) {
        
        mlen = recv(socket, buff, sizeof(buff),0);
        if(mlen == 0){
	            
	   	   
	   if ( -1 == sigprocmask(SIG_BLOCK, &mask, &old) ) {
	       perror("sigprocmask");
	       exit(EXIT_FAILURE);
	   }
	   if ( -1 == sigaction(SIGINT, &act, NULL) ) {
	     perror("sigaction");
	     exit(EXIT_FAILURE);
	   }

           write_to_output(END_CALL, strlen(END_CALL));
	   sigsuspend(&old);  
	   stop_windowing();
           return; 
        } else {
          write_to_output(buff, mlen);
        }
	
      }
    
      if (fds[LOCAL].revents & POLLIN ) {
           
        update_input_buffer();
        
        if(has_whole_line() || has_hit_eof()){  
	  len =  read_from_input(buff,MAXLINE);         
	  send(socket, buff, len, 0); 
       }  
     }
    
    } while (!has_hit_eof());
    
    
    stop_windowing();
   
    
}

int clinent_chat(char *hostname, int port, int a, int v, int N) {
  int sock;                   /* socket descriptors */
  struct sockaddr_in address;
  struct hostent  *host;
  char buff[MAXLEN+1], name[MAXLEN+1];
   
  /* figure out who we're talking to */
  if (! (host = gethostbyname(hostname)) ){
     printf("why am I hear in  hostname\n");
     perror("hostname");
     exit(EXIT_FAILURE);
  }
  
  
  /* Create the socket */
  if( -1 == (sock = socket(AF_INET, SOCK_STREAM, 0))){
    printf("why am I hear in socket\n");
    perror("socket");
    exit(EXIT_FAILURE);

  }
 
  /* connect it */
  address.sin_family = AF_INET;
  address.sin_port   = htons(port);                /* use our port  */
  address.sin_addr.s_addr   = *(uint32_t*)host->h_addr_list[0];  /* net order */


  if( -1 == (connect(sock, (struct sockaddr *)&address, sizeof(address))) ){
     printf("why am I hear in connert\n");
     perror("connect");
     exit(EXIT_FAILURE);
  }

  /* send name to the server */
  cuserid(name);
  send(sock, name, strlen(name), 0);  
  if(!a) 
     printf("Whating for response from %s\n", host->h_name);

  recv(sock, buff, sizeof(buff), 0);
 
  if( (strcmp(buff, CONFERM)) ){
    printf("%s decline connection\n", host->h_name);
    close(sock);
    return -1;

  }


  chatclient(sock, v, N); 

  /* send some data */
  close(sock);
  return 0;
}


int verify_connectin(int sock, int a){
  int i;
  socklen_t len;
  struct sockaddr_in sockinfo;
  char addr[INET_ADDRSTRLEN];
  char answer[MAXLEN+1], name[MAXLEN+1];
  
  if (a){
    recv(sock, name, sizeof(name), 0);
    send(sock, CONFERM, (strlen(CONFERM) + 1), 0);
    return 1;
  }
   
  /* now we're in business, I suppose */
  len = sizeof(sockinfo);
  getsockname(sock, (struct sockaddr *) &sockinfo, &len);

  /* find out about the new socket and the other end */
  inet_ntop(AF_INET, &sockinfo.sin_addr.s_addr, addr, sizeof(addr));
  
  /* get the name of the client */
  recv(sock, name, sizeof(name), 0);  
  
  printf("Mytalk requests form %s@%s. Accept (y/n)?\n",  name, addr);

  scanf("%s", answer);

  for(i = 0; i < (strlen(answer)); i++){  
      answer[i] = tolower(answer[i]);  

  } 
   
 
  if(!strcmp(answer, "y") || !strcmp(answer, "yes")){
   
    send(sock, CONFERM, (strlen(CONFERM) + 1), 0);
    return 1;
  } else { 
    send(sock, DECLINE, (strlen(DECLINE)+1), 0);
    return 0;
  }

}

void chat_server(int port, int a, int v, int N) {
  int sock, newsock;
  struct sockaddr_in address, newsockinfo, peerinfo;
  socklen_t len;
  
 
  
  if( -1 == (sock = socket(AF_INET, SOCK_STREAM, 0))){
    printf("why am I hear in socket\n");
    perror("socket");
    exit(EXIT_FAILURE);

  }

  address.sin_family        = AF_INET;
  address.sin_port          = htons(port);       /* use our port  */
  address.sin_addr.s_addr   = htonl(INADDR_ANY); /* all local interfaces */
   
  if(-1 == (bind(sock, (struct sockaddr *)&address, sizeof(address)))){
    printf("why am I hear in bind\n");
    perror("bind");
    exit(EXIT_FAILURE);

  }

  if ( -1 == (listen(sock,DEFAULT_BACKLOG))) {  /* listen */
    printf("why am I hear in listen\n");
    perror("listeny");
    exit(EXIT_FAILURE);

  }

  len = sizeof(newsockinfo);    /* accept */

    
  if(-1 == (newsock = accept(sock, (struct sockaddr *)&peerinfo, &len))){
    printf("why am I hear in accept\n");
    perror("accept");
    exit(EXIT_FAILURE);

  }
  
  if(verify_connectin(newsock, a)){


    chatclient(newsock, v, N); 

  }


  close(sock);                /* all done, clean up */
  close(newsock);
 
}
