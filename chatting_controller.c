#include "header.h"

extern int fds[2];

extern int client_list[max_number+1];
extern struct login_info user_info[max_number+1];
extern void * chatting_renew();			// thread

char chat_list[15][300];



extern void *chatting_controller(){                //PIPE(FDS) DELIVER

	char buf[buf_size];
	char msg[1500];
	char tmp[10];
	char * ptr;
	int i,pnum;
	pthread_t t_id;

pthread_create(&t_id,NULL,chatting_renew,NULL);              // Timer_THREAD CREATE
pthread_detach(t_id);


while(1){   // WHILE START
memset(buf,0,sizeof(buf));

read(fds[0],buf,sizeof(buf));

memset(msg,0,sizeof(msg));
sscanf(buf,"%d",&pnum);

ptr=strtok(buf," ");
ptr=strtok(NULL," ");
while(ptr!=NULL){
	strcpy(&msg[strlen(msg)],ptr);
	strcpy(&msg[strlen(msg)]," ");
ptr=strtok(NULL," ");
}



strcpy(tmp, user_info[pnum].name);
memset(buf,0,sizeof(buf));

strcpy(&buf[strlen(buf)],"["	);
strcpy(&buf[strlen(buf)],tmp	);
strcpy(&buf[strlen(buf)],"]: "	);
strcpy(&buf[strlen(buf)],msg	);
strcpy(&buf[strlen(buf)],"\n"	);


if(strlen(msg)==0 && user_info[pnum].introduce==0){
memset(buf,0,sizeof(buf));
strcpy(&buf[strlen(buf)],"              !!!!! ["	);
strcpy(&buf[strlen(buf)],tmp	);
strcpy(&buf[strlen(buf)],"] is in to server !!!!!\n"	);
user_info[pnum].introduce=1;
}

else if(strlen(msg)==0 && user_info[pnum].introduce==1){
memset(msg,0,sizeof(msg) );
strcpy(&msg[strlen(msg) ],"\n\n\n\n\n\n\n\n\n//////////////////////////////Chatting room ////////////////////////////////////\n\n" );

for(i=0;i<15;i++)
if(chat_list[i][0]==0 )
strcpy(&msg[strlen(msg) ],"\n" ); 
else
strcpy(&msg[strlen(msg) ],chat_list[i] );

strcpy(&msg[strlen(msg) ],"////////////////////////////////////////////////////////////////////////////////\nCreated by Ingirl v.0715\nCommand list = /help  exit  = /exit\n\n*@ Chatting room is automatically renewed every 60 sec\n*@ Or if you press 'enter' key Chatting room is rewed\nInput: " );

write(client_list[pnum],msg,sizeof(msg) );
continue;
}





for(i=0;i<15;i++)
strcpy( chat_list[i], chat_list[i+1]);

strcpy(chat_list[14], buf);
chat_list[14][strlen(chat_list[14] ) ] =0;


memset(msg,0,sizeof(msg) );

strcpy(&msg[strlen(msg) ],"\n\n\n\n\n\n\n\n\n//////////////////////////////Chatting room ////////////////////////////////////\n\n" );


for(i=0;i<15;i++)
if(chat_list[i][0]==0 )
strcpy(&msg[strlen(msg) ],"\n" ); 
else
strcpy(&msg[strlen(msg) ],chat_list[i] );

strcpy(&msg[strlen(msg) ],"////////////////////////////////////////////////////////////////////////////////\nCreated by Ingirl v.0715\nCommand list = /help  exit  = /exit\n\n*@ Chatting room is automatically renewed every 60 sec\n*@ Or if you press 'enter' key Chatting room is rewed\nInput: " );


write(client_list[pnum],msg,sizeof(msg) );





} // WHILE END

}



