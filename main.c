#include "header.h"

////////////////////////////////// extern function declare ////////////////////////////////////

extern void * chatting_controller();			// thread
extern void * server_controller();			// thread
extern void * command_controller();			// thread
extern void * timer_60();				//timer
extern void zombie_kill(int sig);			//zombie process controller
extern void error_handle(char *message);		//erroe handle

///////////////////////////////////////////////////////////////////////////////////////////


int client_list[max_number+1]={0,};		//connected client socket list

int fds[2];
int stat[2];
int cmd[2];
int attack_result[2];

struct login_info user_info[max_number+1]={0,};		//User's information




//////////////////////////////////////////////////////// Main start ///////////////////////////////////////////


int main(int argc,char** argv){

/////////////////////////Variable Declare////////////////////////

int server_sock,client_sock;
struct sockaddr_in server_addr,client_addr;

pthread_t t_id,t_id2,t_id3,t_id4;

socklen_t sock_len;
pid_t pid;
struct sigaction act;
int str_len, i, pnum, option=1,temp;
int fd;

time_t current;

pipe(fds);
pipe(stat);
pipe(cmd);
pipe(attack_result);

char buf[buf_size];
char buf2[buf_size];
char msg[buf_size];

char tmp[20];

FILE * fp=NULL;

/////////////////// Declare End ////////////////////////////

if(argc!=2)
	error_handle("using: programname port");

act.sa_handler=zombie_kill; // SIGNAL HANDLING(SIG_CHLD)
sigemptyset(&act.sa_mask);
act.sa_flags=0;
sigaction(SIGCHLD,&act,0);    //SIGNAL HANDLING END

system("/etc/init.d/rinetd restart 1>/dev/null 2>/dev/null");

if( (fp=popen("systemctl status rinetd.service","r"))==NULL )		//RINETD SETTING
	error_handle("popen error");


memset(buf,0,sizeof(buf));
while( fgets(buf,sizeof(buf),fp)!=NULL ){
sscanf(buf,"%s %s",msg,buf);
if(!strcmp("Active:",msg)){

	if(!strcmp("active",buf))
		break;
	else	
		error_handle("need rinetd have started");
	}
}

pclose(fp);




fd=open("/etc/rinetd.conf",O_WRONLY|O_CREAT|O_TRUNC,0666);		//CREATE ROUTING TABLE
if(fd==-1)
	error_handle("CREATE ERROR");

close(fd);




server_sock=socket(PF_INET,SOCK_STREAM,0);  // socket setting
setsockopt(server_sock,SOL_SOCKET,SO_REUSEADDR,(void*)&option,sizeof(option));// don't time wait


memset(&server_addr,0,sizeof(server_addr));
server_addr.sin_family=AF_INET;
server_addr.sin_addr.s_addr=htonl(INADDR_ANY);
server_addr.sin_port=htons(atoi(argv[1]));  // end setting

if(bind(server_sock,(struct sockaddr*)&server_addr,sizeof(server_addr))==-1)
	error_handle("bind error");

if(listen(server_sock,5)==-1)
	error_handle("listen error");   // Set LISTENING 
////////////////////////////// Make Thread /////////////////////////////


pthread_create(&t_id,NULL,chatting_controller,NULL);              // PARENT PROCESSOR MAKE THREAD
pthread_detach(t_id);

pthread_create(&t_id2,NULL,server_controller,NULL);              // PARENT PROCESSOR MAKE THREAD
pthread_detach(t_id2);

pthread_create(&t_id3,NULL,command_controller,NULL);              // PARENT PROCESSOR MAKE THREAD
pthread_detach(t_id2);

pthread_create(&t_id4,NULL,timer_60,NULL);            		  // PARENT PROCESSOR MAKE THREAD
pthread_detach(t_id4);

///////////////////////////////////////////////////////////////////////




/////////////////// Event slot/////////////////////////////////////////

client_list[max_number]=60;
strcpy(user_info[max_number].name,"sandbag" );
//strcpy(user_info[max_number].addr,"" );
//user_info[max_number].status=1;


////////////////////////////////////////////////////////////////////////




while(1){ /////////////////////////// WHILE START!! /////////////////////////////////

sock_len=sizeof(client_addr);
client_sock=accept(server_sock,(struct sockaddr*)&client_addr,&sock_len);
	if(client_sock==-1)
	continue;

option=1;
setsockopt(client_sock, SOL_SOCKET, SO_KEEPALIVE, &option, sizeof(option) );


/*  option
pid=fork();
if(pid==0){
system("mpg123 ring.mp3 1>/dev/null 2>/dev/null");
return 0;
}
*/




for(i=0;i<max_number;i++){	   // Select p(rocessor)num(ber)
if(client_list[i]==0){
	pnum=i;
	client_list[i]=pnum+1;
	break;
}			//if
}                           // end





for(i=0;i<max_number;i++)					//addr overlap checking
if( !strcmp(inet_ntoa(client_addr.sin_addr) , user_info[i].addr  ) ){

client_list[pnum]=0;
pnum=i;

write(client_list[pnum],"\n\nDetected same ip access\n Good bye\n",37);

memset(&user_info[pnum].name,0,10);
memset(&user_info[pnum].addr,0,20);


memset(buf,0,sizeof(buf));
memset(tmp,0,sizeof(tmp));
sprintf(tmp,"%d",user_info[pnum].child_pid);
strcpy(buf,"kill -9 ");
strcpy(&buf[strlen(buf)], tmp);

system(buf);
}






pid=fork(); 
if(pid==-1){
close(client_sock);
continue;
}

else if(pid==0){    ////////////////////////////// CHILD PROCESSOR CODE/////////////////////////////////////

close(server_sock);
write(client_sock,"\n\npress enter\n\n",16);
read(client_sock,buf,sizeof(buf));			

close(fds[0]);
close(stat[0]);
close(cmd[0]);
close(attack_result[1]);


					 //input user_info.name 
					 

write(client_sock,"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\ninput your login_name\nat least 3 letter and max 10 letter\nonly english or number\n\n\n\n\n\n\n",103);


while(1){//////////////////////////////////WHILE START////////////////////////////////////////////////////
write(client_sock,"name: ",7);
memset(buf,0,sizeof(buf));

if(read(client_sock,buf,sizeof(buf))==0 ){                   // Force Closing
sprintf(msg,"%d /exit",pnum);		//Fucking bitch chicken
write(cmd[1],msg,sizeof(msg));
exit(1);
}


else if(buf[0]==-1 && buf[1]==-12  && buf[2]==-1  ){	//ctrl + c	

sprintf(msg,"%d /exit",pnum);
write(cmd[1],msg,sizeof(msg));
exit(1);
}




buf[strlen(buf)-2]=0;

for(i=0;i<strlen(buf);i++)			//check hangul
if(buf[i]<0 ){
write(client_sock,"\n\nSorry only english or number\nplz reconnect\n\n",47);

sprintf(msg,"%d /exit",pnum);
write(cmd[1],msg,sizeof(msg));
exit(1);
}




for(i=0;i<max_number;i++)
if(	!strcmp(buf,user_info[i].name	)	){ 		//checking overlap
write(client_sock,"ID overlapped\n\n",16);
i=100;
break;
}

if(i==100)
	continue;


if(strlen(buf)>=3&&strlen(buf)<=10){		// checking length and charset

for(i=0;i<strlen(buf);i++)

if( !( (buf[i]>=48 && buf[i]<=57)||(buf[i]>=65 && buf[i]<=90)||(buf[i]>=97 && buf[i]<=122) ) ){
write(client_sock,"only english and number\n\n",24);
break;
} //if


if(i==strlen(buf))
break;


}		// if
else
write(client_sock,"\ntoo long or too short\n\n",25);

}/////////////////////////////////////WHILE END////////////////////////////////////////////////////////




				/////////input user_info///////

if(strlen(buf)==10){
sprintf(msg,"0%s",buf);
strcpy(&msg[strlen(msg) ], inet_ntoa(client_addr.sin_addr) );
}
else{
sprintf(msg,"%d%s",strlen(buf),buf);
strcpy(&msg[strlen(msg) ], inet_ntoa(client_addr.sin_addr) );
}

sprintf(buf,"%d insert %s",pnum, msg);
write(stat[1],buf,sizeof(buf));





while(1){	///////////////////////////WHILE START/////////////////////////////////////////////////////////
memset(buf,0,sizeof(buf));
memset(msg,0,sizeof(msg));

if(read(client_sock,buf,sizeof(buf))==0 ){                   
sprintf(msg,"%d /exit",pnum);		// Force Closing
write(cmd[1],msg,sizeof(msg));
exit(1);
}


if(buf[0]==-1){				// 	ctrl + d 
sprintf(msg,"%d /exit",pnum);
write(cmd[1],msg,sizeof(msg));
exit(1);
}



buf[strlen(buf)-2]=0;	



if(buf[0]==-1 && buf[1]==-12  && buf[2]==-1  ){	//ctrl + c	

sprintf(msg,"%d /exit",pnum);
write(cmd[1],msg,sizeof(msg));
exit(1);
}


///////////////////////// press enter ////////////////////////////////////

if(strlen(buf)==0 ){
sprintf(msg,"%d ",pnum);

write(fds[1],msg,sizeof(msg));
continue;
}

///////////////////////////   hangul support  //////////////////////////////////////////

temp=0;

for(i=0;i<strlen(buf);i++)		//	if hangul
if(   (	  (buf[i]<=(-56) && buf[i]>=(-80) && (buf[i+1]<=-2) && buf[i+1]>=(-95) )     || (buf[i]==(-92) && (buf[i+1]<=-2) && buf[i+1]>=(-95) ) 	) ||  (buf[i]>=0 &&  buf[i]<=127)	){

if(  !strncmp(&buf[0],"/" ,1) )		//if command
break;




if(strlen(buf)==1	&&	!(buf[i]>=0 &&  buf[i]<=127  )	){		//if error  hangul
temp=50;
break;
}

temp=100;			//normal text

if( buf[i]<=(-56) && buf[i]>=(-80)	|| buf[i]==-92 )
i++; 

}		//if end



else{
temp=150;
break;
}					
				//For  end



if(temp==50){		//error text

sprintf(msg,"%d ",pnum);

write(fds[1],msg,sizeof(msg));
continue;
}



if(temp==100){		//normal text

sprintf(msg,"%d ",pnum);
strcpy(&msg[strlen(msg)],buf);

write(fds[1],msg,sizeof(msg));
continue;
}


if(temp==150){

sprintf(msg,"%d ",pnum);

write(fds[1],msg,sizeof(msg));
continue;
}



////////////////////////////////////////// input check /////////////////////////////////////

time(&current);
sprintf(tmp,"%d",current);
memset(msg,0,sizeof(msg));
sprintf(msg,"%d time %s",pnum, tmp);		//last time synchronize
write(stat[1],msg,sizeof(msg));





if(!strncmp(buf,"/attack",7)){ 			//	if /attack
memset(tmp,0,sizeof(tmp));
sscanf(buf,"%s %s",msg,tmp);
sprintf(buf,"%s%s",msg,tmp);

memset(msg,0,sizeof(msg));
sprintf(msg,"%d ",pnum);
strcpy(&msg[strlen(msg)],buf);

write(cmd[1],msg,sizeof(msg));

memset(msg,0,sizeof(msg));
read(attack_result[0],msg,sizeof(msg));

write(client_sock,msg,sizeof(msg));

}


else if(!strcmp(buf,"/exit")){ 			//	if /exit 
sprintf(msg,"%d %s",pnum,buf);
write(cmd[1],msg,sizeof(msg));
exit(1);
}

		
else if( !strncmp(buf,"/",1)  ){	
 					//	if first letter '/'
sprintf(msg,"%d %s",pnum,buf);
write(cmd[1],msg,sizeof(msg));
}




}  		//WHILE STOP

}/////////////////////////////// CHILD PROCESSOR END///////////////////////////////////////////////////////////



else{        //SERVER PROCESSOR CODE

time( &user_info[pnum].last_time  );		// access time 

client_list[pnum]=dup2(client_sock,pnum+30);

user_info[pnum].child_pid=pid;

close(client_sock);



}          // SERVER PROCESSOR END


}			//WHILE END

}      //////////////////////////////////////////////  MAIN END   ////////////////////////////////////////////




