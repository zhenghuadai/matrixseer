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
#define DZHDEB
#include "netlib.h"
#include "debprintf.h"
//#include "lib.h"
//#define MAIN_HS
#define HOSTNAMEERR 1
#define FILENOTFOUND 2 
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

/*
 * 第1行：方法，请求的内容，HTTP协议的版本<BR>下载一般可以用GET方法，请求的内容是 “/index.html”，HTTP协议的版本是指浏览器支持的版本，对于下载软件来说无所谓，所以用1.1版 “HTTP/1.1”；“GET /index.html HTTP/1.1”
 * 第2行：主机名，格式为“Host:主机”
 * 第3行：接受的数据类型，下载软件当然要接收所有的数据类型，所以：<BR>“Accept:* / *”
 * 第4行：指定浏览器的类型:User- Agent:Mozilla/4.0 (compatible; MSIE 5.00; Windows 98)
 * 第5行：连接设置<BR>设定为一直保持连接：“Connection:Keep- Alive”<BR><BR>
 * 第6行：若要实现断点续传则要指定从什么位置起接收数据，格式如下：“Range: bytes=起始位置 - 终止位置” 比如要读前500个字节可以这样写：“Range: bytes=0 - 499”；从第 1000 个字节起开始下载：Range: bytes=999 -
 * 最后，别忘了加上一行空行，表示请求头结束。整个请求头如下：
 * GET /index.html HTTP/1.1
 * Host:www.sina.com.cn
 * Accept:* / *
 * User-Agent:Mozilla/4.0 (compatible; MSIE 5.00; Windows 98)
 * Connection:Keep-Alive
 * Range: bytes=999 -
 *
 * */
int httpget(char * url,http_t *m_http,int *errcode)
{
    int sockfd, recvbytes,sendbytes,totalrecvbytes=0;
    char databuf[1024];
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
    //printf("%s:%s\n",baseurl,inet_ntoa(serv_addr.sin_addr));
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
    sprintf(databuf,"GET /%s HTTP/1.0\r\nAccept: text/plain; text/html\r\nHOST:%s\r\n\r\n",secondaryurl,baseurl);
    debprintf("request:\n%s",databuf);
    if((sendbytes=send(sockfd,databuf,strlen(databuf),0))==-1){
        DEBprintf("socket err");
        if(errcode)	*errcode=HOSTNAMEERR;
        return 0;
    }
#ifdef DEB
    printf("send...\n");
#endif
    headba=tailba=NULL;
        if ((recvbytes=recv(sockfd, databuf, 1023, 0)) ==-1) {
            DEBprintf("socket err");
            if(errcode)	*errcode=HOSTNAMEERR;
            return 0;
        }
        databuf[recvbytes]=0;
        {
            char * findPos = strstr(databuf,"\r\n\r\n");
            if(findPos !=NULL) 
            contentStart = findPos - databuf + 4;
            else 
            contentStart = recvbytes;
        }
        m_http -> head = (char *) malloc( contentStart +1);
        memcpy(m_http->head,databuf,contentStart);
        m_http->head[contentStart]=0;

        tmpba=(bufarray*)malloc(sizeof(bufarray));
        //tmpba=new bufarray;
        tmpba->buf=(char *)malloc((recvbytes+1-contentStart)*sizeof(char));
        tmpba->buflen=0;
        tmpba->next=NULL;
        memcpy(tmpba->buf,databuf+contentStart,recvbytes-contentStart);
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

int httpget(char * url,http_t *m_http,int start,int end,int *errcode)
{
    int sockfd, recvbytes,sendbytes,totalrecvbytes=0;
    char databuf[1028];
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
    //printf("%s:%s\n",baseurl,inet_ntoa(serv_addr.sin_addr));
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
    if(end == -1){
        if(start ==-1){
            sprintf(databuf,"GET /%s HTTP/1.0\r\nAccept: */*\r\nHOST:%s\r\n\r\n",
                    secondaryurl,baseurl);
        }
        else{
            sprintf(databuf,"GET /%s HTTP/1.0\r\nAccept: */*\r\nHOST:%s\r\nRange:bytes=%d-\r\n\r\n",
                    secondaryurl,baseurl,start);
        }
    }
    else{
        sprintf(databuf,"GET /%s HTTP/1.0\r\nAccept: */*\r\nHOST:%s\r\nRange:bytes=%d-%d\r\n\r\n",
                secondaryurl,baseurl,start,end);
    }
    debprintf("request:\n%s",databuf);
    if((sendbytes=send(sockfd,databuf,strlen(databuf),0))==-1){
        DEBprintf("socket err");
        if(errcode)	*errcode=HOSTNAMEERR;
        return 0;
    }
#ifdef DEB
    printf("send...\n");
#endif
    headba=tailba=NULL;
    if ((recvbytes=recv(sockfd, databuf, 1024, 0)) ==-1) {
        DEBprintf("socket err");
        if(errcode)	*errcode=HOSTNAMEERR;
        return 0;
    }
    databuf[recvbytes]=0;
    {
        char * findPos = strstr(databuf,"\r\n\r\n");
        if(findPos !=NULL) 
            contentStart = findPos - databuf + 4;
        else 
            contentStart = recvbytes;
    }
    m_http -> head = (char *) malloc( contentStart +1);
    memcpy(m_http->head,databuf,contentStart);
    m_http->head[contentStart]=0;

    tmpba=(bufarray*)malloc(sizeof(bufarray));
    //tmpba=new bufarray;
    tmpba->buf=(char *)malloc((recvbytes+1-contentStart)*sizeof(char));
    tmpba->buflen=0;
    tmpba->next=NULL;
    memcpy(tmpba->buf,databuf+contentStart,recvbytes-contentStart);
    if(!headba)headba=tmpba;
    //tmpba->buf[recvbytes -contentStart] = '\0';
    tmpba->buflen=recvbytes-contentStart;
    tailba=tmpba;
    totalrecvbytes+=(recvbytes-contentStart);	
#if 1
    while(recvbytes>0)
    {
        tmpba=(bufarray*)malloc(sizeof(bufarray));
        tmpba->buf=(char *)malloc((MAXDATASIZE)*sizeof(char));
        tmpba->buflen=0;
        tmpba->next=NULL;
        if ((recvbytes=recv(sockfd, tmpba->buf, MAXDATASIZE, 0)) ==-1) {
            DEBprintf("socket err");
            if(errcode)	*errcode=HOSTNAMEERR;
            return 0;
        }
        //tmpba->buf[recvbytes] = '\0';
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
    //content[totalrecvbytes]=0;
#ifdef DEB
    //	printf("Received: %d \n %s",recvbytes,content);
    printf("recv...\n");
#endif
    close(sockfd);
    m_http->content = content ;
    return 1;
}


int httpDownLoad(char * url,char* fn,int start)
{
    int sockfd, recvbytes,sendbytes,totalrecvbytes=0;
    char databuf[1028];
    char baseurl[64];
    struct hostent *host;
    struct sockaddr_in serv_addr;
    char *content;
    int curbeg=0;
    char *secondaryurl;
    int i=0;
    int contentStart;
    FILE* fp;
    int errcode;
    int fileLen = 0;
    if(start>0){
        fp  = fopen(fn,"a");
        if(fp==NULL) {printf("open file %s err\n",fn);return -1;}
        debprintf("appand \n");
        //fseek(fp,start,SEEK_SET);
        //fseek(fp,0,SEEK_END);
    }
    else{
        fp  = fopen(fn,"w");
        if(fp==NULL) {printf("open file %s err\n",fn);return -1;}
    }
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
    debprintf("base:%s||||\n%s\n",baseurl,secondaryurl);
#endif
    if((host=gethostbyname(baseurl))==NULL) {
        DEBprintf("gethostbyname出错！");
        errcode=HOSTNAMEERR;
        return errcode;
        //		exit(1);
    }
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        DEBprintf("socket err");
        errcode=HOSTNAMEERR;
        //	perror("socket创建出错！");
        //	exit(1);
    }
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_port=htons(SERVPORT);
    serv_addr.sin_addr = *((struct in_addr *)host->h_addr);
    //memcpy(&(serv_addr.sin_addr),host->haddr,sizeof(struct in_addr));
    serv_addr.sin_addr = *((struct in_addr *)(host->h_addr));
#ifdef DEB
    debprintf("%s:%s\n",baseurl,inet_ntoa(serv_addr.sin_addr));
#endif
    bzero(&(serv_addr.sin_zero),8);
    if (connect(sockfd, (struct sockaddr *)&serv_addr,sizeof(struct sockaddr)) == -1) {
        printf("connect出错！");
        DEBprintf("socket err");
        errcode=HOSTNAMEERR;
        return errcode;	
        //	exit(1);
    }
#ifdef DEB
    printf("connect...\n");
#endif
    if(start ==-1){
        sprintf(databuf,"GET /%s HTTP/1.1\r\nAccept: */*\r\nHOST:%s\r\n\r\n",
                secondaryurl,baseurl);
    }
    else{
        sprintf(databuf,"GET /%s HTTP/1.1\r\nAccept: */*\r\nHOST:%s\r\nRange:bytes=%d-\r\n\r\n",
                secondaryurl,baseurl,start);
    }
    debprintf("request:\n%s",databuf);
    if((sendbytes=send(sockfd,databuf,strlen(databuf),0))==-1){
        DEBprintf("socket err");
        errcode=HOSTNAMEERR;
        return errcode;
    }
#ifdef DEB
    printf("send...\n");
#endif
    if ((recvbytes=recv(sockfd, databuf, 1024, 0)) ==-1) {
        DEBprintf("socket err");
        errcode=HOSTNAMEERR;
        return errcode;
    }
    databuf[recvbytes]=0;
    {
        char * findPos = strstr(databuf,"\r\n\r\n");
        if(findPos !=NULL) 
            contentStart = findPos - databuf + 4;
        else 
            contentStart = recvbytes;
    }
    char *head = (char *) malloc( contentStart +1);
    memcpy(head,databuf,contentStart);
    head[contentStart]=0;
    debprintf("%s\n",head);

    char* posContent_Length = strstr(head,"Content-Length");
    if(posContent_Length == NULL) {
    fileLen = -1; 
    }
    else{
    fileLen = atoi(posContent_Length + 15);
    }
    debprintf("fileLen:%d\n",fileLen);
    fwrite(databuf+contentStart,recvbytes-contentStart,1,fp);
    totalrecvbytes+=(recvbytes-contentStart);	
    char* recvbuf=(char *)malloc((MAXDATASIZE)*sizeof(char));
    if(fileLen == -1)
    {
        while(recvbytes>0)
        {
            if ((recvbytes=recv(sockfd, recvbuf, MAXDATASIZE, 0)) ==-1) {
                DEBprintf("socket err");
                errcode=HOSTNAMEERR;
                return errcode;
            }
            totalrecvbytes+=recvbytes;
            fwrite(recvbuf,recvbytes,1,fp);
        }
    }else{
        while(totalrecvbytes < fileLen)
        {
            if ((recvbytes=recv(sockfd, recvbuf, MAXDATASIZE, 0)) ==-1) {
                DEBprintf("socket err");
                errcode=HOSTNAMEERR;
                return errcode;
            }
            totalrecvbytes+=recvbytes;
            fwrite(recvbuf,recvbytes,1,fp);
        }
    }
    //	printf("Received: %d \n %s",recvbytes,content);
    debprintf("recv:%d\n",totalrecvbytes);
    fclose(fp);
    free(recvbuf);
    free(head);
    close(sockfd);
    return 1;
}

int httpDownLoad(char * url,char* fn)
{
    int start  = -1;
#ifdef LINUX 
    // LINUX
    struct stat fsc;
    fsc.st_size = -1;
    stat(fn, &fsc);
    start= fsc.st_size;
#endif
    return httpDownLoad(url,fn,start);
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
