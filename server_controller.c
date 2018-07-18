#include "header.h"


extern int stat[2];
extern int client_list[max_number+1];
extern struct login_info user_info[max_number+1];


void * server_controller(){                //PIPE(STAT) DELIVER


int pnum,i,z,temp;
char buf[buf_size];
char msg[buf_size];
char tmp[11];
char * ptr;
char type[10];
time_t current;


while(1){   // WHILE START
memset(msg,0,sizeof(buf));
read(stat[0],msg,sizeof(buf));

sscanf(msg,"%d %s %s",&pnum,type,buf);




if(!strcmp("insert",type) ){ 			/////insert


memset(tmp,0,sizeof(tmp));
sprintf(tmp,"%c",buf[0]);
temp=atoi(tmp);

if(temp==0)
temp=10;


for(i=1;i<=temp;i++)
tmp[i-1]=buf[i];

tmp[i]=0;



strcpy(user_info[pnum].name, tmp);


for(z=0;i<=strlen(buf);i++){
tmp[z]=buf[i];
z++;
}


strcpy(user_info[pnum].addr, tmp);


memset(msg,0,sizeof(msg));
msg[0]=0xc0;
msg[1]=0xcc;
msg[2]=0xb0;
msg[3]=0xcd;
msg[4]=0xc0;
msg[5]=0xcc;
msg[6]=0x20;
msg[7]=0xba;
msg[8]=0xb8;
msg[9]=0xc0;
msg[10]=0xcc;
msg[11]=0xbd;
msg[12]=0xc5;
msg[13]=0xb4;
msg[14]=0xd9;
msg[15]=0xb8;
msg[16]=0xe9;
msg[17]=0x20;
msg[18]=0xc7;
msg[19]=0xd1;
msg[20]=0xb1;
msg[21]=0xdb;
msg[22]=0xc0;
msg[23]=0xcc;
msg[24]=0x20;
msg[25]=0xc1;
msg[26]=0xf6;
msg[27]=0xbf;
msg[28]=0xf8;
msg[29]=0xb5;
msg[30]=0xc7;
msg[31]=0xb4;
msg[32]=0xc2;
msg[33]=0xb0;
msg[34]=0xcc;
msg[35]=0xb4;
msg[36]=0xcf;
msg[37]=0xb4;
msg[38]=0xd9;
msg[39]=0x2e;
msg[40]=0;



memset(buf,0,sizeof(buf));
strcpy(buf,"\n\n\n\n\n\n\n\n\n////////////////////// Hello Your Login Name = [");
strcpy(&buf[strlen(buf)],user_info[pnum].name);
strcpy(&buf[strlen(buf)],"] ///////////////////////\n\n\n           *<This Server support Hangul>*\n\n");
strcpy(&buf[strlen(buf)], msg);
strcpy(&buf[strlen(buf)],"\n/ \\ \n |\n -- If you can see this Hangul support Hangul\n\n");
strcpy(&buf[strlen(buf)],"    Else if you can't see that Hangul follow this\n\n\n");
strcpy(&buf[strlen(buf)],"         case 1. If you Linux \n");
strcpy(&buf[strlen(buf)],"Add this command 'luit -encoding eucKR -- ' before command 'telnet ~~~'\n");
strcpy(&buf[strlen(buf)],"Example 'luit -encoding eucKR -- telnet ingirl.shop 18' \n\n");
strcpy(&buf[strlen(buf)],"         case 2. If you Windows\n");
strcpy(&buf[strlen(buf)],"Change your telnet program's charset to 'euc-kr'\n");
strcpy(&buf[strlen(buf)],"\n\n\n\nGo to Server = Press 'enter'");





write(client_list[pnum],buf,sizeof(buf));


time(&current);
printf("\n%s",ctime(&current));
printf("++++Login\npnum = %d\nlogin name = %s\nip = %s\n\n",pnum, user_info[pnum].name,  user_info[pnum].addr);
/////////////// SERVER ALARM////////////////////////
}




else if(!strcmp("port",type) ){			///////port

ptr=strtok(msg," ");
ptr=strtok(NULL," ");
ptr=strtok(NULL," ");

strcpy(user_info[pnum].target_port ,ptr	);
}




else if(!strcmp("time",type) )			////// time
user_info[pnum].last_time=atoi(buf);












}  		//WHILE END

}





