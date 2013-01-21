#include <stdio.h>
#include <stdlib.h>
#include <sys/un.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <signal.h>
#include "chat_server.h"
struct sockaddr_in addrs[MAXES]={0};
int login[MAXES]={0};
void Chat_server::init()
{
    serverSocket.init();
}

void Chat_server::run()
{
    while(1)
    {
        char	buff[200];
        int n;
        socklen_t client_len=16;//sizeof(client_addr);
        struct sockaddr_in client_addr;
        memset(buff,0,200);
        if((n=serverSocket.recvfrom(buff,200,0,(struct sockaddr*)&client_addr,&client_len))<0)
        {
            perror("recv err\n");
            exit(0);
        }
        buff[n]='\0';
        fprintf(stdout,"<%s",buff);
        processMsg(buff,client_addr);
        //		fprintf(stdout,"addr:%d,port:%d\n",client_addr.sin_addr.s_addr,client_addr.sin_port);
        //		fprintf(stdout,">%s",buff);
        memset(buff,0,200);
    }
}


int Chat_server::processMsg(char *buf,struct sockaddr_in addr)
{
    int	tolen=sizeof(addr);
    char who[4];
    int i;
    int slen=strlen(buf);
    int cmd=0;
    if(slen<4){

        sprintf(buf,"sorry");
        if( serverSocket.sendto(buf,strlen(buf),0,(struct sockaddr*)&addr,tolen) !=0){

        }
        return 0;
    }


    for(i=0;i<3;i++)
        who[i]=buf[i];
    who[i]='\0';
    if(buf[i]=='q')cmd=LOGOFF;
    printf("recv %s\n",who);
    if(strcmp(who,DZH)==0)
    {
        if(cmd==LOGOFF)
        {
            login[DZHTH]=0;
            return 1;
        }
        addrs[DZHTH]=addr;
        login[DZHTH]=1;
        if(login[NCTH])
        {
           serverSocket.sendto(buf,strlen(buf),0,(struct sockaddr*)&addrs[NCTH],tolen);
        }
        else
        {
            sprintf(buf,"NC does not login");
            printf(buf,"NC does not login");
            if(serverSocket.sendto(buf,strlen(buf),0,(struct sockaddr*)&addrs[DZHTH],tolen) <0){
                printf("send back err\n");
            };

        }
        return 1;
    }
    if(strcmp(who,NC)==0)
    {
        if(cmd==LOGOFF)
        {
            login[NCTH]=0;
            return 1;
        }
        addrs[NCTH]=addr;
        login[NCTH]=1;
        if(login[DZHTH])
        {
            serverSocket.sendto(buf,strlen(buf),0,(struct sockaddr*)&addrs[DZHTH],tolen);
        }
        else
        {
            sprintf(buf,"DZH does not login");
            serverSocket.sendto(buf,strlen(buf),0,(struct sockaddr*)&addrs[NCTH],tolen);
        }
        return 1;
    }
    sprintf(buf,"sorry");
    serverSocket.sendto(buf,strlen(buf),0,(struct sockaddr*)&addr,tolen);
    return 1;
}
