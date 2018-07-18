#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <fcntl.h>
#include <time.h>


#define max_number 30
#define buf_size 1000



struct login_info{ 			//user_info 's struct
char name[11];
char addr[20];
int status;
char target[20];
char target_port[200];
int attackby[max_number+2];
int introduce;
pid_t child_pid;
time_t last_time;
};





