#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>





void zombie_kill(int sig){ //	I WILL KILL ALL ZOMBIE!!!!!!! WITH THIS PENUTS!!!!!!!!!! 
pid_t pid;
int status;
pid=waitpid(-1,&status,WNOHANG);
//printf:
}




void error_handle(char *message){ // CALL ME PARROT......
fputs(message,stderr);
fputc('\n',stderr);
exit(1);
}


