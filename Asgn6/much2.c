#include "much2.h"

int cd_cmd(clstage stage){ 

  if(stage->argc < 2){
    /* take me home */
    if(chdir(getenv("HOME")) == -1){
      perror("cd");
      return -1;
    }

  } else {
    if(chdir(stage->argv[1]) == -1){
      perror("cd");
      return -1;
    }
  }
 

  return 0;
}

int close_pips(int fd[][2], int length){
  int i;
  for(i = 0; i < (length-1); i++){
    close(fd[i][READ_END]);
    close(fd[i][WRITE_END]);
  }  
  return 0;

}

int pipeline_cmd(pipeline cl){

  int fd[(cl->length - 1)][2];
  pid_t pids[cl->length];
  int status[cl->length];  
  clstage stage;  
  int i, infile, outfile;
  
  stage = cl->stage;    

  /* buld pipes */
  for(i = 0; i < (cl->length); i++){
   
    if(pipe(fd[i]) == -1){
      perror("pipe");
      exit(EXIT_FAILURE);
    }
  }
  
  /* make Prosseses */
  for(i = 0; i < (cl->length); i++){
    /* while(stage){ */
  
    /* printf("%s \n", stage[i].argv[0]);*/    
    
     if((pids[i] = fork()) == -1){
            perror("fork");
            exit(EXIT_FAILURE);
     }
      
     /* child prosses stuf */
     if(pids[i] == 0){
      
      /* handle pipes and redirection */

        if(stage[i].outname){

	    if( -1 == (outfile = open(stage[i].outname,
                                  O_WRONLY | O_CREAT |
                                  O_TRUNC, S_IRUSR | S_IWUSR))){
	        perror(stage[i].outname);
                exit(EXIT_FAILURE);
             }

        } else {
        
            /* if this is the last stage in the pipe right to stout */
	  if(i < (cl->length - 1)){
	        outfile = fd[i][WRITE_END];
            } else {
	        outfile =  STDOUT_FILENO;
            }
        }

        if(stage[i].inname){

	     if( -1 == (infile = open(stage[i].inname, O_RDONLY))){
	        perror(stage[i].inname);
	        exit(EXIT_FAILURE);
	      }
     
        } else {
           
            /* first stage dosent read from pipe*/      
            if(i != 0){
                infile = fd[i-1][READ_END];
	    } else {
	        infile = STDIN_FILENO;
	    } 
   
        }

        /* Dup2 over the stuff */        
       if(dup2(infile, STDIN_FILENO) == -1){
               perror(infile);
               exit(EXIT_FAILURE);
       }

       if(dup2(outfile, STDOUT_FILENO) == -1){
               perror(outfile);
               exit(EXIT_FAILURE);
       }
      
       close_pips(fd, (cl->length));

       /* do the program */

       if(execvp(stage[i].argv[0], stage[i].argv) == -1){
	 perror(stage[i].argv[0]);
         exit(EXIT_FAILURE);

       }
       return 0;
    }
    
    /*increment the while loop */
    /*i++;
    stage = stage->next;
    printf("%s \n", stage->argv[0]);
    */
  }


  close_pips(fd, (cl->length));    

  for(i = 0; i < (cl->length); i++){
    
    waitpid(pids[i], &status[i], 0);

  }  
  
  return 0;

}
