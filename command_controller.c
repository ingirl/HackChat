#include "header.h"


extern int cmd[2];
extern int stat[2];
extern int fds[2];
extern int attack_result[2];

extern int client_list[max_number+1];
extern struct login_info user_info[max_number+1];
extern void error_handle(char *message);		//erroe handle



void * command_controller(){                //PIPE(cmd) DELIVER

int pnum,i,z,j,k,fd,count;
char buf[buf_size];
char buf2[buf_size];
char msg[buf_size];
char port_list[500]={0,};
char tmp[20];
char src_addr[20];
char dst_addr[20];
char *ptr;
pid_t pid;
char pid_num[4][6];
time_t current,start;

FILE * fp;
FILE * fp_tmp;



while(1){   // WHILE START
memset(msg,0,sizeof(msg));
read(cmd[0],msg,sizeof(msg));

pid_t pid;

sscanf(msg,"%d %s",&pnum,buf);


if(strncmp(buf,"/",1)){
continue;
}

if(!strcmp("/exit",buf) ){    				//exit



fp=fopen("/etc/rinetd.conf","r");		///etc/rinetd.conf edit
if(fp==NULL)
	error_handle("open error");

fp_tmp=fopen("/etc/rinetd.conf_tmp","w");	
if(fp==NULL)
	error_handle("open error");

while(fgets(buf,sizeof(buf),fp)!=NULL ){ //while start
sscanf(buf,"%s %d %s",src_addr,&i,dst_addr);
if(!strcmp(dst_addr,  user_info[pnum].addr)|| i/((pnum+1)*1000)==1	){
	continue;
}
fputs(buf,fp_tmp);
memset(buf,0,sizeof(buf));
}					//while end


fclose(fp);					// edit end
fclose(fp_tmp);					// edit end

if(rename("/etc/rinetd.conf_tmp","/etc/rinetd.conf")==-1) 		//rename
	error_handle("rename error");



for(i=0;i<max_number;i++){
if(user_info[pnum].attackby[i]==0	)
break;

memset(buf,0,sizeof(buf));
strcpy(buf,"\n\n\n                      !!!!!Notice!!!!!\n          your attack target [ ");
strcpy(&buf[strlen(buf)],user_info[pnum].name	);
strcpy(&buf[strlen(buf)]," ] disconnected \n\n"	);
strcpy(&buf[strlen(buf)],"\n\n[");
strcpy(&buf[strlen(buf)],user_info[	( user_info[pnum].attackby[i] )-1 	].name);
strcpy(&buf[strlen(buf)],"]: ");
write(client_list[	( user_info[pnum].attackby[i] )-1 	],buf,sizeof(buf));

memset(user_info[      ( user_info[pnum].attackby[i] )-1       ].target,0,20);
memset(user_info[      ( user_info[pnum].attackby[i] )-1       ].target_port,0,200);
memset(user_info[      ( user_info[pnum].attackby[i] )-1       ].attackby,0,(max_number)*4);
}




printf("----Logout\nname = %s\nip = %s\n\n", user_info[pnum].name,  user_info[pnum].addr);

write(client_list[pnum],"\n\n\nGood Bye\n\n",14);

close(client_list[pnum]);
client_list[pnum]=0;
memset(&user_info[pnum],0,sizeof(struct login_info));




}




else if(!strncmp(buf,"/attack",7)){	//////////////////////attack///////////////////////////////////////

if(buf[7]==0){			//if '/attack' ////////////////////////////////
memset(buf,0,sizeof(buf));
strcpy(buf,"\n\n\n\n\n\n\n\n\n\n\n\n/////////////////////////////you are under attack by//////////////////////////\n\n");

for(i=0;i<max_number;i++){
if(user_info[pnum].attackby[i]==0	)
continue;

strcpy(&buf[strlen(buf)], "                              name: ");
strcpy(&buf[strlen(buf)], user_info[( user_info[pnum].attackby[i] )-1 ].name		);
strcpy(&buf[strlen(buf)], "\n");

}


strcpy(&buf[strlen(buf)], " \n\n\n\n\n///////////your attack targe id is [ ");
strcpy(&buf[strlen(buf)], user_info[pnum].target);
strcpy(&buf[strlen(buf)], " ] and target's opened port is////////////\n\n"	);

strcpy(port_list, user_info[pnum].target_port);
ptr=strtok(port_list,"\n");

i=1;

while(ptr!=NULL){

	memset(buf2,0,sizeof(buf2));
	sprintf(buf2 ,"      ( [ServerIp]'s Port %d )  ---Forwarding--->  ( [%s]'s Port %s )  \n", ( ( (pnum+1)*1000) +i), user_info[pnum].target, ptr  );


	strcpy(&buf[strlen(buf)], buf2);
	ptr=strtok(NULL,"\n");
	i++;
}               //while end
memset(port_list,0,sizeof(port_list));


strcpy(&buf[strlen(buf)],"\n\nReturn chatting room room room = press enter\nInput: ");

write(attack_result[1],buf,sizeof(buf));

continue;
}
////////////////////////////////////////////////////////////////////////////////

if(user_info[pnum].target[0]!=0){ 		//already have target



fp=fopen("/etc/rinetd.conf","r");               ///etc/rinetd.conf edit
if(fp==NULL)
	                error_handle("open error");

fp_tmp=fopen("/etc/rinetd.conf_tmp","w");    
if(fp==NULL)
	                error_handle("open error");

while(fgets(msg,sizeof(msg),fp)!=NULL ){ //while start
sscanf(msg,"%s %d %s",src_addr,&i,dst_addr);
if( !strcmp(dst_addr,  user_info[pnum].addr)|| i/((pnum+1)*1000)==1  ){  
	                        continue;
		        }
       fputs(msg,fp_tmp);
        memset(msg,0,sizeof(msg));
}                                       //while end


fclose(fp);                                     // edit end
fclose(fp_tmp);                                 // edit end

if(rename("/etc/rinetd.conf_tmp","/etc/rinetd.conf")==-1)               //rename
		        error_handle("rename error");

for(i=0;i<max_number+1;i++){
if(client_list[i]!=0	)
	if(!strcmp(user_info[i].name, user_info[pnum].target	)	)
	{
		for(j=0;j<max_number+1;j++)
			if(user_info[i].attackby[j]==(pnum+1)	)
				user_info[i].attackby[j]=0;
	}
}		//for end


memset(user_info[pnum].target,0,20);
memset(user_info[pnum].target_port,0,200);


} /////////////////////// already have target ///////////////////////////////



for(i=0;i<strlen(buf);i++)
buf[i]=buf[i+7];



for(i=0;i<max_number;i++){
if(client_list[i]!=0){				
if(!strcmp(buf, user_info[i].name)){
	if(user_info[i].status==1){			//if attack able







for(z=0;z<max_number;z++){
if(user_info[i].attackby[z]==0	){
user_info[i].attackby[z]=pnum+1;
break;
}

}

j=i;
k=z;

strcpy(	user_info[pnum].target, user_info[i].name		);


write(client_list[pnum],"\nNow scanning wait....\nThis operation may take time.........\nDon't move!!!!\nAt least take 30 sec.....",101);

pid=fork();

if(pid==0){  				//CHILD START

strcpy(dst_addr,user_info[i].addr);
strcpy(src_addr,"0.0.0.0");



count=0;
time(&start);
while(1){//////// 20sec while /////




memset(&pid_num,0,24);

pid=fork();

if(pid==0){
memset(buf,0,sizeof(buf));
strcpy(buf,"nmap -sS -P0 ");                  //NMAP -sS -P0
strcpy(&buf[strlen(buf)],dst_addr);
strcpy(&buf[strlen(buf)],">nmap_result_");
strcpy(&buf[strlen(buf)],user_info[pnum].name);
strcpy(&buf[strlen(buf)],"1");
strcpy(&buf[strlen(buf)]," && echo success>result_");
strcpy(&buf[strlen(buf)],user_info[pnum].name);
strcpy(&buf[strlen(buf)],"1");
system(buf);
while(1){
	}
}

sprintf(pid_num[1],"%d",pid);

pid=fork();

if(pid==0){
memset(buf,0,sizeof(buf));
strcpy(buf,"nmap -sT -P0 ");                  //NMAP -sT -P0
strcpy(&buf[strlen(buf)],dst_addr);
strcpy(&buf[strlen(buf)],">nmap_result_");
strcpy(&buf[strlen(buf)],user_info[pnum].name);
strcpy(&buf[strlen(buf)],"2");
strcpy(&buf[strlen(buf)]," && echo success>result_");
strcpy(&buf[strlen(buf)],user_info[pnum].name);
strcpy(&buf[strlen(buf)],"2");
system(buf);
while(1){
	}
}


sprintf(pid_num[2],"%d",pid);

pid=fork();

if(pid==0){
memset(buf,0,sizeof(buf));
strcpy(buf,"nmap -sS ");                  //NMAP -sS 
strcpy(&buf[strlen(buf)],dst_addr);
strcpy(&buf[strlen(buf)],">nmap_result_");
strcpy(&buf[strlen(buf)],user_info[pnum].name);
strcpy(&buf[strlen(buf)],"3");
strcpy(&buf[strlen(buf)]," && echo success>result_");
strcpy(&buf[strlen(buf)],user_info[pnum].name);
strcpy(&buf[strlen(buf)],"3");
system(buf);
while(1){
	}
}

sprintf(pid_num[3],"%d",pid);

////////////////////////////////////////////////////////////////////////////////////



while(1){                       //SCAN COMPLETE 

memset(buf,0,sizeof(buf));
strcpy(buf,"result_");
strcpy(&buf[strlen(buf)],user_info[pnum].name);
strcpy(&buf[strlen(buf)],"1");

if(!access(buf,F_OK)){
	z=1;
break;
}

memset(buf,0,sizeof(buf));
strcpy(buf,"result_");
strcpy(&buf[strlen(buf)],user_info[pnum].name);
strcpy(&buf[strlen(buf)],"2");

if(!access(buf,F_OK)){
	z=2;
break;
}

memset(buf,0,sizeof(buf));
strcpy(buf,"result_");
strcpy(&buf[strlen(buf)],user_info[pnum].name);
strcpy(&buf[strlen(buf)],"3");

if(!access(buf,F_OK)){
	z=3;
break;
}

}



for(i=1;i<4;i++){
strcpy(buf,"kill -9 ");
strcpy(&buf[strlen(buf)],pid_num[i]	 );
//strcpy(&buf[strlen(buf)]," 2>/dev/null"	 );
system(buf);
}



fp=popen("ps -ef|grep nmap","r");
if(fp==NULL)
	error_handle("popen error");

while(fgets(buf,sizeof(buf),fp)!=NULL	){
sscanf(buf,"%s %s",msg,tmp);
memset(buf,0,sizeof(buf));
strcpy(buf,"kill -9 ");
strcpy(&buf[strlen(buf)],tmp	 );
strcpy(&buf[strlen(buf)]," 2>/dev/null"	 );
system(buf);
}


for(i=1;i<4;i++){
memset(buf,0,sizeof(buf));
sprintf(tmp,"%d",i);


strcpy(buf,"result_");
strcpy(&buf[strlen(buf)],user_info[pnum].name);
strcpy(&buf[strlen(buf)],tmp);

unlink(buf);
}





sprintf(tmp,"%d",z);

memset(buf,0,sizeof(buf));
strcpy(buf,"nmap_result_");
strcpy(&buf[strlen(buf)],user_info[pnum].name);
strcpy(&buf[strlen(buf)],tmp	);


fp=fopen(buf,"r");   		 //READ SCAANING RESULT
if(fp==NULL)
	error_handle("open error");


memset(buf,0,sizeof(buf));
while(fgets(buf,sizeof(buf),fp)!=NULL ){

sscanf(buf,"%s %s",msg,tmp);


if(!strcmp(tmp,"open") || !strcmp(tmp,"filtered")	)	{
for(i=0;i<6;i++){
if(msg[i]=='/'	){
	port_list[strlen(port_list)]='\n';
	break;
}
port_list[strlen(port_list)]=msg[i];
		}  //for end
} 			//if end
memset(msg,0,sizeof(msg));
}			 //WHILE end

fclose(fp);


for(i=1;i<4;i++){
memset(buf,0,sizeof(buf));
sprintf(tmp,"%d",i);


strcpy(buf,"nmap_result_");
strcpy(&buf[strlen(buf)],user_info[pnum].name);
strcpy(&buf[strlen(buf)],tmp);

unlink(buf);
}



if(port_list[0]==0){		//No opne port

time(&current);
if(current-start<20)
count++;
else
count=3;


}
else
break;




if(count==3){
user_info[j].attackby[k]=0	;

memset(user_info[pnum].target,0,20);
memset(user_info[pnum].target_port,0,200);

memset(buf,0,sizeof(buf));
strcpy(&buf[strlen(buf)],"\n\nNo opened port sorry....");
strcpy(&buf[strlen(buf)],"\n\n\n[");
strcpy(&buf[strlen(buf)],user_info[pnum].name);
strcpy(&buf[strlen(buf)],"]: ");

write(attack_result[1],buf,sizeof(buf));
return 0;
}



sleep(1);
}		//////// 20sec while /////





port_list[strlen(port_list)-1]=0;

sprintf(msg,"%d port ",pnum);	
strcpy(&msg[strlen(msg)],port_list);
write(stat[1],msg,sizeof(msg));




fd=open("/etc/rinetd.conf",O_WRONLY|O_APPEND);    //PORT FORWARING TABLE WRITE
if(fd==-1)
	error_handle("open error");


i=1;

ptr=strtok(port_list,"\n");
while(ptr!=NULL){
memset(buf,0,sizeof(buf));
sprintf(buf,"%s %d %s %s\n",src_addr,( ( (pnum+1)*1000  )+i ),dst_addr,ptr 	);

write(fd,buf,strlen(buf));
ptr=strtok(NULL,"\n");
i++;
}
close(fd);

while(1){///////////////////////WHILE START/////////////////////////////////////////////

while(1){
if( (fp=popen("/etc/init.d/rinetd restart","r"))==NULL )
	error_handle("popen error");

memset(buf,0,sizeof(buf));
fgets(buf,sizeof(buf),fp);
if(strlen(buf)!=0)	break;
}


pclose(fp);

if(strlen(buf)<=51){	///////////////////////RESTART SUCCESS///////////////////////////////////

memset(buf,0,sizeof(buf));
strcpy(buf,"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n                      !!!!!Notice!!!!!\n          you will be attacked by [ ");
strcpy(&buf[strlen(buf)],user_info[pnum].name	);
strcpy(&buf[strlen(buf)]," ]  \n\n\n\n\n\n"	);
strcpy(&buf[strlen(buf)],"\nReturn chatting room room room = press enter\nInput: ");

write(client_list[j],buf,sizeof(buf));		// waring 


memset(buf,0,sizeof(buf));
strcpy(buf,"\n\n\n\n\n\n\n\n\n\n\n\n\n\nConnected!!\n\n Server'ip address is forwaring to target's ip address\ntarget's opened port list\n\n");


ptr=strtok(msg," ");
ptr=strtok(NULL," ");
ptr=strtok(NULL," ");
ptr=strtok(ptr,"\n");

i=1;


while(ptr!=NULL){

memset(buf2,0,sizeof(buf2));
sprintf(buf2 ,"      ( [ServerIp]'s Port %d )  ---Forwarding--->  ( [%s]'s Port %s )  \n", ( ( (pnum+1)*1000) +i), user_info[pnum].target, ptr	);


strcpy(&buf[strlen(buf)], buf2);
ptr=strtok(NULL,"\n");
i++;
}		//while end


strcpy(&buf[strlen(buf)], "\nReturn chatting room room = press enter\nInput: " )	;


write(attack_result[1],buf,sizeof(buf));
break;
}/////////////////////////////////////RESTART SUCCESS END///////////////////////////////



else{	/////////////////////////RESTART FAIL/////////////////////////
sleep(1);
printf("Restart failed maybe so fast\nlen= %d\nbuf=%s\n",strlen(buf),buf);

}
///////////////////////////////RESTART FAIL END/////////////////////////


}//////////////////////////WHILE END////////////////////////////////////////////
return 0;
}			//CHILD END


break;

	}
	else{ 					// attack disable				
memset(buf,0,sizeof(buf));
strcpy(buf,"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nHis state is Deny\nsorry\nReturn chatting room room = press enter\nInput: ");


write(attack_result[1],buf,sizeof(buf));
break;
	}
					}
}//if end
}// for end
if(i==30){
memset(buf,0,sizeof(buf));
strcpy(buf,"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nThis ID is not being accessed.\nReturn chatting room room = press enter\nInput: ");

write(attack_result[1],buf,sizeof(buf));
}

} 				//attack end

else if(!strcmp("/list",buf) ){                             // list 
memset(buf,0,sizeof(buf));
strcpy(&buf[strlen(buf)],"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nCurrent Access User List\n\n");

for(i=0;i<max_number+1;i++){
if(client_list[i]!=0){
strcpy(&buf[strlen(buf)],"name: ");
strcpy(&buf[strlen(buf)],user_info[i].name);

strcpy(&buf[strlen(buf)],"        Attack available: ");

if(user_info[i].status==0)
strcpy(&buf[strlen(buf)],"deny\n");
else{
strcpy(&buf[strlen(buf)],"permit\n");

}	//if
}		//for

}//////////////////// for /////////////////////////////

strcpy(&buf[strlen(buf)],"\n\nReturn chatting room room = press enter\nInput: ");

write(client_list[pnum],buf,sizeof(buf));

}		//if end


else if(!strcmp("/help",buf) ){			//help
memset(buf,0,sizeof(buf));
strcpy(buf,"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n/list = Show Current Access Number and attack permit status\n\n/attack = SHow current my attack target and user who attack you\n\n/attack 'username' = connect with 'username'\n\n/enable = change can be attack mode\n\n/disable = change can't be attack mode\n\n\n\nReturn chatting room room = press enter\nInput: ");


write(client_list[pnum],buf,sizeof(buf));
}


else if(!strcmp("/enable",buf) ){                          // enable
user_info[pnum].status=1;

memset(buf,0,sizeof(buf));
sprintf(buf,"%d ",pnum);
write(fds[1],buf,sizeof(buf) );
}


else if(!strcmp("/disable",buf) ){			//disable




fp=fopen("/etc/rinetd.conf","r");               ///etc/rinetd.conf edit
if(fp==NULL)
	        error_handle("open error");

fp_tmp=fopen("/etc/rinetd.conf_tmp","w");    
if(fp==NULL)
	        error_handle("open error");

while(fgets(buf,sizeof(buf),fp)!=NULL ){ //while start
	sscanf(buf,"%s %d %s",src_addr,&i,dst_addr);
	if( !strcmp(dst_addr,  user_info[pnum].addr)|| i/((pnum+1)*1000)==1  ) 
		        continue;
	
	fputs(buf,fp_tmp);
	memset(buf,0,sizeof(buf));
}                                       //while end


fclose(fp);                                     // edit end
fclose(fp_tmp);                                 // edit end

if(rename("/etc/rinetd.conf_tmp","/etc/rinetd.conf")==-1)               //rename
        error_handle("rename error");



for(i=0;i<max_number;i++){
	if(user_info[pnum].attackby[i]==0       )
		break;

	memset(buf,0,sizeof(buf));
	strcpy(buf,"\n\n\n                      !!!!!Notice!!!!!\n          your attack target [ ");
	strcpy(&buf[strlen(buf)],user_info[pnum].name   );  
	strcpy(&buf[strlen(buf)]," ] changed disable mode \n\n" );
	strcpy(&buf[strlen(buf)],"\n\n[");
	strcpy(&buf[strlen(buf)],user_info[     ( user_info[pnum].attackby[i] )-1       ].name);
	strcpy(&buf[strlen(buf)],"]: ");
	write(client_list[      ( user_info[pnum].attackby[i] )-1       ],buf,sizeof(buf));
}


for(i=0;i<max_number;i++){
if(user_info[pnum].attackby[i]==0	)
continue;

memset(user_info[      ( user_info[pnum].attackby[i] )-1       ].target,0,20);
memset(user_info[      ( user_info[pnum].attackby[i] )-1       ].target_port,0,200);
//memset(user_info[      ( user_info[pnum].attackby[i] )-1       ].attackby,0,(max_number)*4);dd
}


memset(user_info[pnum].target,0,20);
memset(user_info[pnum].target_port,0,200);
memset(user_info[pnum].attackby,0,(max_number)*4);
user_info[pnum].status=0;

memset(buf,0,sizeof(buf));
sprintf(buf,"%d ",pnum);
write(fds[1],buf,sizeof(buf) );
}





else{						//else
memset(buf,0,sizeof(buf));
sprintf(buf,"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nnot designated command\n\n\nReturn chatting room room room = press enter\nInput: ",user_info[pnum].name);
write(client_list[pnum],buf,sizeof(buf));

}

}      //WHILE END

}
