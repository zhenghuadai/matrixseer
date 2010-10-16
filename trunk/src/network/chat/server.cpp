#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <signal.h>
#define DZH "DZH"
#define NC "NC"
#define DZHTH 1
#define NCTH 0
#define MAXES 10
#define LOGOFF 8
#define PORT 1236
int server;
void quitp(int);
struct sockaddr_in addrs[MAXES];
int processMsg(char *buf,struct sockaddr_in addr);
int login[MAXES];
int main()
{
    int ret;
    socklen_t server_len=16;
    struct sockaddr_in server_addr,client_addr;
    int i;
    memset(addrs,0,MAXES * sizeof(struct sockaddr_in));
    memset(login,0,MAXES * sizeof(int));
    if((server=socket(AF_INET,SOCK_DGRAM,0))<0)
    {
        perror("socket err\n");
        exit(0);
    }
    signal(SIGQUIT,quitp);
    server_addr.sin_family=AF_INET;
    server_addr.sin_addr.s_addr=htonl(0);
    server_addr.sin_port=htons(PORT);
    if(bind(server,(struct sockaddr*)&server_addr,16) !=0)
    {
        perror("bind err\n");
        exit(0);
    }
    if((ret = getsockname(server,(struct sockaddr *)&server_addr,&server_len)) !=0)
    {
        printf("getsockname err %d\n", ret);
        exit(0);
    }
    printf("server use port%d\n",ntohs(server_addr.sin_port));
    while(1)
    {
        char	buff[200];
        int n;
        socklen_t client_len=16;//sizeof(client_addr);
        memset(buff,0,200);
        if((n=recvfrom(server,buff,200,0,(struct sockaddr*)&client_addr,&client_len))<0)
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
    close(server);
}
void quitp(int sig)
{
    if(sig==SIGQUIT)
        close(server);
}
int processMsg(char *buf,struct sockaddr_in addr)
{
    int	tolen=sizeof(addr);
    char who[4];
    int i;
    int slen=strlen(buf);
    int cmd=0;
    if(slen<4){

        sprintf(buf,"sorry");
        if( sendto(server,buf,strlen(buf),0,(struct sockaddr*)&addr,tolen) !=0){

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
            sendto(server,buf,strlen(buf),0,(struct sockaddr*)&addrs[NCTH],tolen);
        }
        else
        {
            sprintf(buf,"NC does not login");
            printf(buf,"NC does not login");
            if(sendto(server,buf,strlen(buf),0,(struct sockaddr*)&addrs[DZHTH],tolen) <0){
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
            sendto(server,buf,strlen(buf),0,(struct sockaddr*)&addrs[DZHTH],tolen);
        }
        else
        {
            sprintf(buf,"DZH does not login");
            sendto(server,buf,strlen(buf),0,(struct sockaddr*)&addrs[NCTH],tolen);
        }
        return 1;
    }
    sprintf(buf,"sorry");
    sendto(server,buf,strlen(buf),0,(struct sockaddr*)&addr,tolen);
    return 1;
}
