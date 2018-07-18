#include "header.h"

extern int cmd[2];
extern int client_list[max_number+1];
extern struct login_info user_info[max_number+1];
extern char chat_list[15][300];


void * timer_60(){ 				//timer 


int i;
char buf[20];
char tmp[20];
time_t current;


while(1){	//while start


sleep(60);
time(&current);

for(i=0;i<30;i++)
if(client_list[i]!=0 ){		//access client


if(user_info[i].name[0]==0){ 	//access buf noname
if(  (current - user_info[i].last_time) >120){
write(client_list[i],"\n\nTime out!!!!\n",16);

memset(buf,0,sizeof(buf) );
memset(tmp,0,sizeof(tmp) );

sprintf(tmp,"%d",user_info[i].child_pid);
strcpy(buf,"kill -9 ");
strcpy(&buf[strlen(buf)], tmp);

system(buf);

client_list[i]=0;
}
}

else{				//access and have name 
if(  (current - user_info[i].last_time) > 3600 && strcmp(user_info[i].name, "hackme") && strcmp(user_info[i].name, "admin") ){  //21600
write(client_list[i],"\n\nTime out!!!!\n",16);

sprintf(buf,"%d /exit",i);		// Force Closing
write(cmd[1],buf,sizeof(buf));

memset(buf,0,sizeof(buf) );
memset(tmp,0,sizeof(tmp) );
sprintf(tmp,"%d",user_info[i].child_pid);
strcpy(buf,"kill -9 ");
strcpy(&buf[strlen(buf)], tmp);

system(buf);
}
}



			}				//access clinet end





}		//while end
}





void * chatting_renew(){

time_t current,start;
int i;
char msg[buf_size];

while(1){
sleep(60);

memset(msg,0,sizeof(msg) );


strcpy(&msg[strlen(msg) ],"\n\n\n\n\n\n\n\n\n//////////////////////////////Chatting room ////////////////////////////////////\n\n" );


for(i=0;i<15;i++)
if(chat_list[i][0]==0 )
strcpy(&msg[strlen(msg) ],"\n" ); 
else
strcpy(&msg[strlen(msg) ],chat_list[i] );

strcpy(&msg[strlen(msg) ],"////////////////////////////////////////////////////////////////////////////////\nCreated by Ingirl v.0715\nCommand list = /help  exit  = /exit\n\n*@ Chatting room is automatically renewed every 60 sec\n*@ Or if you press 'enter' key Chatting room is rewed\nInput: " );



for(i=0;i<max_number;i++)
if(client_list[i]!=0 && user_info[i].introduce==1  )
write(client_list[i],msg,sizeof(msg) );

}		//while end

}
