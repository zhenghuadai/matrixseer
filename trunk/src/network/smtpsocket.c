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
#include "lib.h"

//#define DZHDEB
//#define SMTP_MAIN
#define PORT 25 //smtp port
#define SIZE 1024 
//define mail commands
#define SMTPEHLO 0
#define SMTPAUTH 1
#define SMTPUSER 2
#define SMTPPASS 3
#define SMTPMAIL 4
#define SMTPRCPT 5
#define SMTPDATA 6
#define SMTPCONT 7
#define SMTPQUIT 8
/*
   const char * smtpserver="ncic.ac.cn";
   const char * smtpsendaddr="dzh@ncic.ac.cn";
   const char * myaccount="dzh";
   const char * smtpserver="smtp.gmail.com";
   const char * smtpsendaddr="djx.zhenghua@gmai.com";
   const char * myaccount="djx.zhenghua@gmail.com";
   const char * mypasswd="djx_zh010";
   */
void base64enc(const char *,char *);
char * base64str(const char *instr);
Email::Email()
{
    smtpserver="smtp.sina.com.cn";
    smtpsendaddr="djx_zh@sina.com";
    myaccount="djx_zh";
    mypasswd="djx_zh010";
}
int Email::mailfrom(char *sender,char * passwd)
{
    myaccount= sender;
    mypasswd= passwd;
}
int Email::mailserver(char *server)
{
    smtpserver=server;
}
int Email::mailto(char *mailaddr,char *data,char *subject)
{
    int sockfd;
    struct sockaddr_in server_addr;
    struct hostent *server_ip;
    int numbytes=0,i=0;
    char username[512]="";//mail username
    char passwd[512]="";//mail passwd

    char buff[512]="",tmp[4]="",ip[SIZE]="";
    int ret=0;//function return
    char *msg[9]={""};
    char *n_return[9]={"250","334","334","235","250","250","354","250","250"}; //return number
    char *hellobuf;
    char *usernamebuf;
    char *passwdbuf;
    char *smtpmailfrombuf;
    char *smtpmailtobuf;
    char *smtpdatabuf;
    hellobuf=(char *)malloc(10+strlen(smtpserver));
    sprintf(hellobuf,"ehlo %s\n",smtpserver);
    msg[SMTPEHLO]=hellobuf;//"ehlo ncic.ac.cn\n";
    msg[SMTPAUTH]=(char *)malloc(12);//"auth login\n";
    sprintf(msg[SMTPAUTH],"auth login\n");
    usernamebuf=base64str(myaccount);
    //base64enc("dzh",username);
    strcat(usernamebuf,"\n");
    msg[SMTPUSER]=usernamebuf;

    //base64enc("djx_zh010",passwd);
    passwdbuf=base64str(mypasswd);
    strcat(passwdbuf,"\n");
    msg[SMTPPASS]=passwdbuf;

    smtpmailfrombuf=(char *)malloc(15+strlen(smtpsendaddr));
    sprintf(smtpmailfrombuf,"mail from:<%s>\n",smtpsendaddr);
    msg[SMTPMAIL]=smtpmailfrombuf;//"mail from:<dzh@ncic.ac.cn>\n";

    smtpmailtobuf=(char *)malloc(15+strlen(mailaddr));
    sprintf(smtpmailtobuf,"rcpt to:<%s>\n",mailaddr);
    msg[SMTPRCPT]=smtpmailtobuf;//"rcpt to:<djx.zhenghua@gmail.com>\n";


    msg[SMTPDATA]=(char *)malloc(6);//"data\n";
    sprintf(msg[SMTPDATA],"data\n");
    smtpdatabuf=(char *)malloc(strlen(data)+5);
    sprintf(smtpdatabuf,"%s\n.\n",data);
    msg[SMTPCONT]=smtpdatabuf;//"are you ok\n.\n";
    msg[SMTPQUIT]=(char *)malloc(6);//"quit\n";
    sprintf(msg[SMTPQUIT],"quit\n");

    /*------------------------------
      connect server,and send command
      ------------------------------*/
    while((server_ip=gethostbyname(smtpserver))==NULL)
    {
        perror("gethostbyname error");
        sleep(300);
    }
    //create a socket
    if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)
    {
        perror("socket error");
        return(-1);
    }
    //address information
    server_addr.sin_family=AF_INET;//host byte order
    server_addr.sin_port=htons(PORT);//short,network byte order
    server_addr.sin_addr=*((struct in_addr *)server_ip->h_addr);//server ip
    bzero(&(server_addr.sin_zero),8);//zero the rest of struct
    //connect server
    if(connect(sockfd,(struct sockaddr *)&server_addr,sizeof(struct sockaddr))==-1)
    {
        perror("connect error");
        return(-1);
    }
    //if connect success,server return "220"
    if((numbytes=recv(sockfd,buff,SIZE,0))==-1)
    {
        perror("recv error");
        return(-1);
    } 
    //clean tmp
    for(i=0;i<4;i++)
        tmp[i]='\0';
    strncpy(tmp,buff,3);
    if(strcmp(tmp,"220")!=0)
        return (-1);

    i=SMTPEHLO;
    while(i<SMTPQUIT) {
        if(msg[i]==NULL)
        {
            i++;
            continue;
        }
        if((numbytes=send(sockfd,msg[i],strlen(msg[i]),0))==-1)
        {
            perror("send error");
            break;
        }
        //sleep(1);we dont have to use it,because recv() can 
        //choke itself until it received data
        if((numbytes=recv(sockfd,buff,SIZE,0))==-1)
        {
            perror("recv error");
            break;
        } 
        strncpy(tmp,buff,3);
#ifdef DZHDEB
        printf("/**********/\ncommand:%s\n",msg[i]);
        printf("return buff:%s\n",buff);
        printf("should return:%s\n",n_return[i]);
#endif
        if(strcmp(tmp,n_return[i])==0) 
            i++;
        else
        {
            printf("server return err\n");
            break;
        }
    }//endwhile


    //send quit to end mail connection
    if((numbytes=send(sockfd,msg[SMTPQUIT],strlen(msg[SMTPQUIT]),0))==-1)
    {
        perror("send error");
        return(-1);
    }

    for(i=0;i<SMTPQUIT;i++)
        free(msg[i]);
    close(sockfd);
    return (0);
}

/*-------------------------
  base64 encode function
  -------------------------*/
char * base64str(const char *instr)
{ 
    char * table="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    int instr_len=0,i=0,j=0,pad=0; 
    unsigned char buf1[4]="",buf2[4]=""; 
    char *outstr;
    instr_len=strlen(instr);
    pad=instr_len%3;
    outstr=(char *)malloc((instr_len/3)*4+6);
    outstr[0]=0;
    for(i=0;i<instr_len;i+=3) { 
        if(i==instr_len-pad)
            strncpy((char *)buf1,&instr[i],pad);
        else
            strncpy((char *)buf1,&instr[i],3);

        buf2[0] = buf1[0] >> 2; 
        buf2[1] = (buf1[0] & 0x03) << 4 | buf1[1] >> 4; 
        buf2[2] = (buf1[1] & 0x0f) << 2 | buf1[2] >> 6; 
        buf2[3] = buf1[2] & 0x3f; 
        for(j=0;j<4;j++) 
            buf2[j]=table[buf2[j]];

        if(i==instr_len-pad)
            for(j=3;j>pad;j--)
                buf2[j]='=';

        strncat(outstr,(char *)buf2,4);
    }
    return outstr;
}

void base64enc(const char *instr,char *outstr)
{ 
    char * table="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    int instr_len=0,i=0,j=0,pad=0; 
    unsigned char buf1[4]="",buf2[4]=""; 

    instr_len=strlen(instr);
    pad=instr_len%3;
    for(i=0;i<instr_len;i+=3) { 
        if(i==instr_len-pad)
            strncpy((char *)buf1,&instr[i],pad);
        else
            strncpy((char *)buf1,&instr[i],3);

        buf2[0] = buf1[0] >> 2; 
        buf2[1] = (buf1[0] & 0x03) << 4 | buf1[1] >> 4; 
        buf2[2] = (buf1[1] & 0x0f) << 2 | buf1[2] >> 6; 
        buf2[3] = buf1[2] & 0x3f; 
        for(j=0;j<4;j++) 
            buf2[j]=table[buf2[j]];

        if(i==instr_len-pad)
            for(j=3;j>pad;j--)
                buf2[j]='=';

        strncat(outstr,(char *)buf2,4);
    }

}
#ifdef SMTP_MAIN
int main(int argc,char *argv[])
{
    char *email="djx.zhenghua@gmail.com";
    char * data="Cfrdjx_zh@htomai.com\nSUBJECT:good luck\ntext plain";
    char *subj="good";
    Email myemail;
    myemail.mailto(email,data,subj);
}
#endif
