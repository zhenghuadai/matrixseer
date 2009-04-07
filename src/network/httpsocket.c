#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <signal.h>
#include <netdb.h>
#include "netlib.h"
#include "debprintf.h"
//#include "lib.h"
//#define DEB
//#define MAIN_HS
#define HOSTNAMEERR 0
int client;
void quitp(int sig);
char *user;
typedef struct bufarray0
{
    char * buf;
    int buflen;
    struct bufarray0 * next;
}bufarray;
#define SERVPORT 80 
#define MAXDATASIZE 4195 /*每次最大数据传输量 */
char *httpget(char * url,int *errcode)
{
    http_t tmp;
    httpget(url,&tmp,errcode);
    return tmp.content;
}
char *httpsock(char * url,int *errcode)
{
    http_t tmp;
    httpget(url,&tmp,errcode);
    return tmp.content;
}
int httpget(char * url,http_t *m_http,int *errcode)
{
    int sockfd, recvbytes,sendbytes,totalrecvbytes=0;
    char sendbuf[1024];
    char baseurl[64];
    struct hostent *host;
    struct sockaddr_in serv_addr;
    char *content;
    int curbeg=0;
    char *secondaryurl;
    int i=0;
    int contentStart;
    bufarray *headba,*tailba;
    bufarray *tmpba;
    secondaryurl=url;
    if(strncmp(url,"http://",7)==0)secondaryurl+=7;
    while(*secondaryurl!='/')
    {
        if(*secondaryurl=='\0')break;
        baseurl[i++]=*secondaryurl;
        secondaryurl++;
    }
    baseurl[i]='\0';
#ifdef DEB
    printf("base:%s||||\n%s\n",baseurl,secondaryurl);
#endif
    if((host=gethostbyname(baseurl))==NULL) {
        DEBprintf("gethostbyname出错！");
        if(errcode)	*errcode=HOSTNAMEERR;
        return 0;
        //		exit(1);
    }
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        DEBprintf("socket err");
        if(errcode)	*errcode=HOSTNAMEERR;
        //	perror("socket创建出错！");
        //	exit(1);
    }
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_port=htons(SERVPORT);
    serv_addr.sin_addr = *((struct in_addr *)host->h_addr);
    //memcpy(&(serv_addr.sin_addr),host->haddr,sizeof(struct in_addr));
    serv_addr.sin_addr = *((struct in_addr *)(host->h_addr));
#ifdef DEB
    printf("%s:%s\n",baseurl,inet_ntoa(serv_addr.sin_addr));
#endif
    bzero(&(serv_addr.sin_zero),8);
    if (connect(sockfd, (struct sockaddr *)&serv_addr,sizeof(struct sockaddr)) == -1) {
        printf("connect出错！");
        DEBprintf("socket err");
        if(errcode)	*errcode=HOSTNAMEERR;
        return 0;	
        //	exit(1);
    }
#ifdef DEB
    printf("connect...\n");
#endif
    sprintf(sendbuf,"GET /%s HTTP/1.0\r\nAccept: text/plain; text/html\r\nHOST:%s\r\n\r\n",secondaryurl,baseurl);
    if((sendbytes=send(sockfd,sendbuf,strlen(sendbuf),0))==-1){
        DEBprintf("socket err");
        if(errcode)	*errcode=HOSTNAMEERR;
        return 0;
    }
#ifdef DEB
    printf("send...\n");
#endif
    headba=tailba=NULL;
        if ((recvbytes=recv(sockfd, sendbuf, 1023, 0)) ==-1) {
            DEBprintf("socket err");
            if(errcode)	*errcode=HOSTNAMEERR;
            return 0;
        }
        sendbuf[recvbytes]=0;
        {
            char * findPos = strstr(sendbuf,"\r\n\r\n");
            if(findPos !=NULL) 
            contentStart = findPos - sendbuf + 4;
            else 
            contentStart = recvbytes;
        }
        m_http -> head = (char *) malloc( contentStart +1);
        memcpy(m_http->head,sendbuf,contentStart);
        m_http->head[contentStart]=0;

        tmpba=(bufarray*)malloc(sizeof(bufarray));
        //tmpba=new bufarray;
        tmpba->buf=(char *)malloc((recvbytes+1-contentStart)*sizeof(char));
        tmpba->buflen=0;
        tmpba->next=NULL;
        memcpy(tmpba->buf,sendbuf+contentStart,recvbytes-contentStart);
        if(!headba)headba=tmpba;
        tmpba->buf[recvbytes -contentStart] = '\0';
        tmpba->buflen=recvbytes-contentStart;
            tailba=tmpba;
        totalrecvbytes+=(recvbytes-contentStart);	
#if 1
    while(recvbytes>0)
    {
        tmpba=(bufarray*)malloc(sizeof(bufarray));
        tmpba->buf=(char *)malloc((MAXDATASIZE+1)*sizeof(char));
        tmpba->buflen=0;
        tmpba->next=NULL;
        if ((recvbytes=recv(sockfd, tmpba->buf, MAXDATASIZE, 0)) ==-1) {
            DEBprintf("socket err");
            if(errcode)	*errcode=HOSTNAMEERR;
            return 0;
        }
        tmpba->buf[recvbytes] = '\0';
        tmpba->buflen=recvbytes;
            tailba->next=tmpba;
            tailba=tmpba;
        totalrecvbytes+=recvbytes;	
        //printf("Received: %d \n %s",recvbytes,tmpba->buf);
    }
#endif
    content=(char *)malloc(sizeof(char )*(totalrecvbytes+1));
    curbeg=0;
    while(headba!=NULL)
    {
        bufarray* tmp1=headba;
        memcpy(content+curbeg,headba->buf,headba->buflen);
        curbeg+=headba->buflen;
        headba=headba->next;
        free(tmp1->buf);
        free(tmp1);
    }
    m_http -> length = totalrecvbytes;
    content[totalrecvbytes]=0;
#ifdef DEB
    //	printf("Received: %d \n %s",recvbytes,content);
    printf("recv...\n");
#endif
    close(sockfd);
    m_http->content = content ;
    return 1;
}
#ifdef MAIN_HS
int main(int argc, char *argv[]){
    char *cont;
    if (argc < 2) {
        fprintf(stderr,"Please enter the server's hostname!\n");
        exit(1);
    }
    cont=httpsock(argv[1]);
    printf("%s",cont);
}
#endif
