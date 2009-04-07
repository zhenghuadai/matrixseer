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
#include <sys/stat.h>
//#define DEB
//#define DZHDEB
#include "netlib.h"
#include "debprintf.h"
#ifdef DEB
#define DEBE(x)\
    x;
#else 
#define DEBE(x)
#endif

//#include "lib.h"
//#define MAIN_HS
#define HOSTNAMEERR -1
#define FILENOTFOUND -2 
#define SENDUSERERR -4
#define SERVERUSERERR -5
#define SENDPASSERR -6 
#define RECVUSERERR -7
#define SERVERPASSERR -8
#define SOCKSENDERR -9
#define SOCKRECVERR -10
#define SERVERERR -11
#define SOCKSUCESS 0
#define SERVPORT 110 
#define MAXLEN 4196 /*每次最大数据传输量 */

typedef struct buf_type
{
    char* buf;
    size_t len;
    void  print(){
        for(int i=0;i<len;i++)
            fprintf(stdout,"%c",buf[i]);
    }
}buf_t;
typedef struct bufarray0
{
    char * buf;
    int buflen;
    struct bufarray0 * next;
}bufarray;

#define safeSend(sockfd,databuf,len,b)\
    if((sendbytes=send(sockfd,databuf,len,b))==-1){\
        debprintf("socket err");\
        return SOCKSENDERR;\
    }

#define safeRecv(sockfd,databuf,len,b)\
    if ((recvbytes=recv(sockfd, databuf, len, 0)) ==-1) {\
        debprintf("socket err");\
        return SOCKRECVERR;\
    }

#define safeCmp(databuf,str,len)\
    if(memcmp(databuf,str,len) != 0){\
        return SERVERERR;\
    }
typedef struct __mail{
    int id;
    char* uidl;
    int isnew;
}inner_mail_t;


inline char* gotoCnext(char* p, char c)
{
    while(*p!=c) p++;
    p++;
    return p;
}

int recvall(int sockfd,int len,buf_t* buf)
{
    int recvbytes;
    int totalrecvbytes=0;
    unsigned int maxlen = len+16;
    char* tbuf=(char *)malloc(sizeof(char )*(maxlen));
    char* curp = tbuf;
    safeRecv(sockfd,tbuf,maxlen,0);
    DEBE(tbuf[recvbytes]= 0);
    //debprintf("list : %s\n",tbuf);
    if((recvbytes>=3)&&(tbuf[recvbytes-3] == 46)&&(tbuf[recvbytes-2] == 13)&&(tbuf[recvbytes-1] == 10)){
        buf->buf =  tbuf;
        buf->len = recvbytes;
        return SOCKSUCESS;
    }

    totalrecvbytes=(recvbytes);
    curp += recvbytes;
    while((recvbytes >0)&& totalrecvbytes < maxlen)
    {
        if ((recvbytes=recv(sockfd, curp, maxlen-totalrecvbytes, 0)) ==-1) {
            DEBprintf("socket recv err");
            return SOCKRECVERR;
            return 0;
        }
        totalrecvbytes+=recvbytes;	
        if((recvbytes>=3)&&(curp[recvbytes-3] == 46)&&(curp[recvbytes-2] == 13)&&(curp[recvbytes-1] == 10))
            break;
        curp += recvbytes;
    }

    buf->buf =  tbuf;
    buf->len = totalrecvbytes;
    return SOCKSUCESS;
}

int recvall(int sockfd,buf_t* buf)
{
    int recvbytes;
    int totalrecvbytes=0;
    char* tbuf = (char*)malloc(10*1024);
    unsigned int maxlen = 1024*10;
    safeRecv(sockfd,tbuf,maxlen,0);
    DEBE(tbuf[recvbytes]= 0);
    if((recvbytes>=3)&&(tbuf[recvbytes-3] == 46)&&(tbuf[recvbytes-2] == 13)&&(tbuf[recvbytes-1] == 10)){
        buf->buf =  tbuf;
        buf->len = recvbytes;
        return SOCKSUCESS;
    }

    bufarray *headba,*tailba;
    bufarray *tmpba;
    tmpba=(bufarray*)malloc(sizeof(bufarray));
    tmpba->buf= tbuf;
    tmpba->buflen=0;
    tmpba->next=NULL;
    headba=tmpba;
    tmpba->buflen=recvbytes;
    tailba=tmpba;
    totalrecvbytes=(recvbytes);	
#if 1
    //while(recvbytes == maxlen)
    while(recvbytes >0)
    {
        tmpba=(bufarray*)malloc(sizeof(bufarray));
        tmpba->buf=(char *)malloc((maxlen)*sizeof(char));
        tmpba->buflen=0;
        tmpba->next=NULL;
        if ((recvbytes=recv(sockfd, tmpba->buf, maxlen, 0)) ==-1) {
            DEBprintf("socket recv err");
            return SOCKRECVERR;
            return 0;
        }
        tmpba->buflen=recvbytes;
        tailba->next=tmpba;
        tailba=tmpba;
        totalrecvbytes+=recvbytes;	
        if((recvbytes>=3)&&(tmpba->buf[recvbytes-3] == 46)&&(tmpba->buf[recvbytes-2] == 13)&&(tmpba->buf[recvbytes-1] == 10))
            break;
    }
#endif
    char* content=(char *)malloc(sizeof(char )*(totalrecvbytes+1));
    int curbeg=0;
    while(headba!=NULL)
    {
        bufarray* tmp1=headba;
        memcpy(content+curbeg,headba->buf,headba->buflen);
        curbeg+=headba->buflen;
        headba=headba->next;
        free(tmp1->buf);
        free(tmp1);
    }

    buf->buf =  content;
    buf->len = totalrecvbytes;
    content[totalrecvbytes] =0;
    return SOCKSUCESS;

}

int recvlen(int sockfd,void** buf,int& len)
{
}

int pop3Uidl(int sockfd,buf_t& buf)
{
    int recvbytes,sendbytes;
    char databuf[1024];
    sprintf(databuf,"uidl \r\n");
    safeSend(sockfd,databuf,strlen(databuf),0);
    int     ret = recvall(sockfd,&buf);
    if(ret != SOCKSUCESS){
        debprintf("recv all err:%d\n",ret);
        return ret;
    }
    //DEBE( printf("recv\n"));
    //DEBE( buf.print());
    return SOCKSUCESS;
}

int pop3Uidl(int sockfd,int mid, char*&  uidlbuf)
{
    int recvbytes,sendbytes;
    char databuf[1024];
    sprintf(databuf,"uidl %d\r\n",mid);
    safeSend(sockfd,databuf,strlen(databuf),0);
    safeRecv(sockfd,databuf,1024,0);
    safeCmp(databuf,"+OK",3);
    DEBE(databuf[recvbytes]= 0);
    debprintf("uidl : %s",databuf);
    char* p=gotoCnext(databuf,' ');
    p = gotoCnext(p,' ') ;
    size_t uidllen = recvbytes -(p - databuf)-2;
    debprintf("%d\n",uidllen);
    uidlbuf=(char*)malloc(uidllen+1);
    memcpy(uidlbuf,p,uidllen);
    (uidlbuf)[uidllen] = 0;
    debprintf("%s\n",uidlbuf);
    return SOCKSUCESS;
}

int pop3List(int sockfd,int mid)
{
    int recvbytes,sendbytes;
    char databuf[1024];
    sprintf(databuf,"list %d\r\n",mid);
    safeSend(sockfd,databuf,strlen(databuf),0);
    safeRecv(sockfd,databuf,1024,0);
    safeCmp(databuf,"+OK",3);
    DEBE(databuf[recvbytes]= 0);
    debprintf("list : %s\n",databuf);
    char* p=gotoCnext(databuf,' ');
    p = gotoCnext(p,' ') ;
    return atoi(p);
}

int  pop3List(int sockfd,buf_t& buf)
{
    int recvbytes,sendbytes;
    char databuf[16];
    sprintf(databuf,"list\r\n");
    safeSend(sockfd,databuf,strlen(databuf),0);
    int     ret = recvall(sockfd,&buf);
    if(ret != SOCKSUCESS){
        debprintf("recv all err:%d\n",ret);
        return ret;
    }
    DEBE( buf.print());
    return SOCKSUCESS;
}

int pop3Stat(int sockfd,int& mails, int& len)
{

    int recvbytes,sendbytes;
    char databuf[1024];
    sprintf(databuf,"stat\r\n");
    safeSend(sockfd,databuf,strlen(databuf),0);
    safeRecv(sockfd,databuf,1024,0);
    safeCmp(databuf,"+OK",3);
    DEBE(databuf[recvbytes]= 0);
    debprintf("list : %s\n",databuf);
    char* p=gotoCnext(databuf,' ');
    mails = atoi(p);
    p = gotoCnext(p,' ') ;
    len= atoi(p);
    return SOCKSUCESS;
}

int  pop3Top(int sockfd,int mid,int lines ,buf_t& buf)
{
    int recvbytes,sendbytes;
    char databuf[16];
    sprintf(databuf,"top %d %d\r\n",mid,lines);
    safeSend(sockfd,databuf,strlen(databuf),0);
    int     ret = recvall(sockfd,&buf);
    if(ret != SOCKSUCESS){
        debprintf("recv all err:%d\n",ret);
        return ret;
    }
    //DEBE( buf.print());
    return SOCKSUCESS;
}

int  pop3Retr(int sockfd,int mid,buf_t& buf)
{
    int recvbytes,sendbytes;
    char databuf[16];
    sprintf(databuf,"retr %d\r\n",mid);
    safeSend(sockfd,databuf,strlen(databuf),0);
    int     ret = recvall(sockfd,&buf);
    if(ret != SOCKSUCESS){
        debprintf("recv all err:%d\n",ret);
        return ret;
    }
    //DEBE( buf.print());
    DEBE(printf("mail len:%d\n",buf.len));
    return SOCKSUCESS;
}

int  pop3Connect(char* pop3url,char* user,char* passwd,int& sock)
{
    int recvbytes,sendbytes;
    struct hostent *host;
    struct sockaddr_in serv_addr;
    int errcode;
    int sockfd;
    char databuf[1024];
    if((host=gethostbyname(pop3url))==NULL) {
        debprintf("gethostbyname出错！");
        errcode=HOSTNAMEERR;
        return errcode;
    }
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        debprintf("create socket err");
        errcode=HOSTNAMEERR;
        return errcode;
    }
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_port=htons(SERVPORT);
    serv_addr.sin_addr = *((struct in_addr *)host->h_addr);
    //memcpy(&(serv_addr.sin_addr),host->haddr,sizeof(struct in_addr));
    serv_addr.sin_addr = *((struct in_addr *)(host->h_addr));
    bzero(&(serv_addr.sin_zero),8);
    if (connect(sockfd, (struct sockaddr *)&serv_addr,sizeof(struct sockaddr)) == -1) {
        printf("connect出错！");
        debprintf("socket err");
        errcode=HOSTNAMEERR;
        return errcode;	
    }
    safeRecv(sockfd,databuf,1024,0);
    safeCmp(databuf,"+OK",3);
    // send user
    sprintf(databuf,"user %s\r\n",user);
    //sprintf(databuf,"user %s\r\npass %s\r\n",user,pass);
    if((sendbytes=send(sockfd,databuf,strlen(databuf),0))==-1){
        debprintf("socket send user err");
        errcode=SENDUSERERR;
        return errcode;
    }
    if ((recvbytes=recv(sockfd, databuf, 1024, 0)) ==-1) {
        debprintf("socket recv user err");
        return RECVUSERERR;
    }
    if(memcmp(databuf,"+OK",3) != 0){
        return SERVERUSERERR;
    }
    databuf[recvbytes]= 0;
    debprintf("%s\n",databuf);
    // send passwd
    sprintf(databuf,"pass %s\r\n",passwd);
    if((sendbytes=send(sockfd,databuf,strlen(databuf),0))==-1){
        debprintf("socket send pass err");
        errcode=SENDPASSERR;
        return errcode;
    }
    if ((recvbytes=recv(sockfd, databuf, 1024, 0)) ==-1) {
        debprintf("socket recv pass err");
        return RECVUSERERR;
    }
    if(memcmp(databuf,"+OK",3) != 0){
        return SERVERPASSERR;
    }
    sock = sockfd;
    return SOCKSUCESS;
}


int pop3Noop(int sockfd)
{
    int recvbytes,sendbytes;
    char databuf[1024];
    sprintf(databuf,"noop\r\n");
    safeSend(sockfd,databuf,strlen(databuf),0);
    safeRecv(sockfd,databuf,1024,0);
    safeCmp(databuf,"+OK",3);
    DEBE(databuf[recvbytes]= 0);
    debprintf("noop: %s\n",databuf);
    return SOCKSUCESS;
}

