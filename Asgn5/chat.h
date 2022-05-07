#ifndef CHATH
#define CHATH
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
#include <talk.h>

#define LOCAL 0
#define REMOTE (LOCAL + 1)
#define MAXLINE 1024
#define MAX_CONNECTIONS 100
#define LISTENER 0

int clinent_chat(char *hostname, int port, int a, int v, int N);
void chat_server(int listener, int a, int v, int N);

#endif
