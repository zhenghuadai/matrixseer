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
#include "threadWrapper.h"
int client;
struct sockaddr_in client_addr,server_addr;
char	buff[200];
socklen_t server_len=sizeof(client_addr);
void quitp(int sig);
char *user;
THREAD_VAR;
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
    fprintf(stdout, "user:%s\n", pn);
    return pn;
}

__kernel recvkernel()
{	
    int n;
    while(1)
    {
        if((n=recvfrom(client,buff,200,0,(struct sockaddr*)&server_addr,&server_len))<0)
        {
            perror("recv err\n");
            close(client);
            exit(0);
        }
        buff[n]='\0';
        fprintf(stdout,"recv: %s\n",buff);
    }
}

int main( int argc, char* argv[])
{
    int client_len;
    int n;
    int myport = 65537;
    signal(SIGQUIT,quitp);
    if((client=socket(AF_INET,SOCK_DGRAM,0))<0)
    {
        perror("socket err\n");
        exit(0);
    }
    if(argc == 2)
        myport = atoi(argv[1]);
    client_addr.sin_family=AF_INET;
    client_addr.sin_addr.s_addr=htonl(0);
    client_addr.sin_port=htons(myport);
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
    server_addr.sin_addr.s_addr=inet_addr("192.168.10.110");
    server_addr.sin_port=htons(1236);
    user=login();
    slaunch0(recvkernel)()
    {	
        while(1)
        {

            memset(buff,0,200);
            sprintf(buff,"%s ",user);
            printf("\n\n-------------------\nplease input you msg:\n");
            fgets(buff+strlen(buff),200,stdin);
            printf("you input msg: %s", buff);
            if(sendto(client,buff,strlen(buff),0,(struct sockaddr*)&server_addr,server_len) <0)
            {
                perror(" send err\n");

                close(client);
                exit(0);
            }
        }//end while
    }
    close(client);
}
void quitp(int sig)
{
    if(sig==SIGQUIT)
        close(client);
}
