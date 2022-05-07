#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<ctype.h>

void usage(const char *name) {
  fprintf(stderr,"usage: %s [ctxvS]f tarfile [ path [... ] ]\n",name);
  exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
  char *flags, *tearfile, *path;
  int c, t ,x, v, f, S;
  int i;

  c = 0;
  t = 0;
  x = 0;
  v = 0;
  f = 0;
  S = 0;
 

  if( (argc == 2) || (argc == 3)){
  
    flags = argv[1];
    tarfile = argv[2];
  
    if( argc == 3)
      path = argv[3];

  } else {
     usage(argv[0]);
  }
  
  /* check the first argument for the flages if one of them is not in
     the flags print a usage message and quit
  */
  for (i = 0; i < strln(flags); i++){

      swich(flags[i]);
    case 'c':
      c++; 
      break;
    case 't':
      t++; 
      break;
    case 'x':
      x++; 
      break;
    case 'v':
      v++; 
      break;
    case 'f':
      f++; 
      break;
    case 'S':
      S++; 
      break;
    default :
     usage(argv[0]);

      }

  }
  
  
  

  return 0;
}
