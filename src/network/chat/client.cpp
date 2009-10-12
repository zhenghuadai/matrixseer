#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include "dqq.h"
int client;
void quitp(int sig);
char *user;
char *login()
{
	char tmp[20];
	char *pn;
	int dt;
	printf("please input your name\n");
	fgets(tmp,19,stdin);
	dt=strlen(tmp);
	if(tmp[dt-1]==10)
	{	
		tmp[dt-1]=0;//tmp[strlen(tmp)-2]==0;
	}
	pn=(char*)malloc(strlen(tmp)+1);
	strcpy(pn,tmp);
	return pn;
}
int main()
{
    int client_len;
    struct sockaddr_in client_addr,server_addr;
    char	buff[200];
    int n;
    socklen_t server_len=sizeof(client_addr);
    signal(SIGQUIT,quitp);
    if((client=socket(AF_INET,SOCK_DGRAM,0))<0)
    {
        perror("socket err\n");
        exit(0);
    }
    client_addr.sin_family=AF_INET;
    client_addr.sin_addr.s_addr=htonl(0);
    client_addr.sin_port=htons(1235);
    if(bind(client,(struct sockaddr*)&client_addr,16)<0)
    {
        perror("bind err\n");
        exit(0);
    }

    //if(getsockname(client,(struct sockaddr *)&client_addr,&client_len)<0)
    //{
    //    perror("get sockname err\n");
    //    exit(0);
    //}
//
    printf("client use port%d\n",ntohs(client_addr.sin_port));

    server_addr.sin_family=AF_INET;
    server_addr.sin_addr.s_addr=inet_addr("192.168.18.12");
    server_addr.sin_port=htons(1236);
    user=login();
    if(fork()==0)
    {	
        while(1)
        {

            memset(buff,0,200);
            //	sprintf(buff,"very good\n");
            sprintf(buff,"%s ",user);
            fgets(buff+strlen(buff),200,stdin);
            if(sendto(client,buff,strlen(buff),0,(struct sockaddr*)&server_addr,server_len)<0)
            {
                perror(" send err\n");

                close(client);
                exit(0);
            }
            memset(buff,0,200);
        }//end while
    }
    else
    {	while(1)
        {
            if((n=recvfrom(client,buff,200,0,(struct sockaddr*)&server_addr,&server_len))<0)
            {
                perror("recv err\n");
                close(client);
                exit(0);
            }
            buff[n]='\0';
            fprintf(stdout,"%s",buff);
        }
    }//else fork()
    close(client);
}
void quitp(int sig)
{
    if(sig==SIGQUIT)
        close(client);
}
