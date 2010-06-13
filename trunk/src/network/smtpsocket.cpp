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
//#define DZHDEB
#include "debprintf.h"
#include "lib.h"

#define SMTPSERVER "smtp.sina.com.cn"
#define MYACCOUNT "djx_zh"
#define MYPASSWD "test123"
//#define SMTP_MAIN
#define PORT 25 //smtp port
#define SIZE 1024 
//define mail commands
#define SMTPEHLO 0
#define SMTPAUTH 1
#define SMTPUSER 2
#define SMTPPASS 3
#define MAILFROM 4
#define SMTPRCPT 5
#define SMTPDATA 6
#define SMTPCONT 7
#define SMTPQUIT 8
const int  BASE64_MAXLINE = 76;
const char EOL[] = "\r\n";

const char mimeTextHeader[] =
"\n--#BOUNDARY#\n"
"Content-Type: text/plain; charset=gb2312\n"
"Content-Transfer-Encoding: quoted-printable\n"
"\n";
const char ATT_HEADER_TEMPLATE[] =
"\n--#BOUNDARY#\n"
"Content-Type: image/jpg; name=%s\n"
"Content-Disposition: attachment; filename=%s\n"
"Content-Transfer-Encoding: base64\n"
"\n";
const char BASE64_TAB[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz0123456789+/";

//"Content-Type: application/octet-stream; name=%s\n"
/*
   const char * smtpserver="ncic.ac.cn";
   const char * smtpsendaddr="dzh@ncic.ac.cn";
   const char * myaccount="dzh";
   const char * smtpserver="smtp.gmail.com";
   const char * smtpsendaddr="djx.zhenghua@gmai.com";
   const char * myaccount="djx.zhenghua@gmail.com";
   const char * mypasswd="djx_zh010";
   */
char* getBaseFilename(const char* fn);
void base64enc(const char *,char *);
char * base64str(const char *instr,int instrSize = 0,int *outSize = 0);
char * ANSIToBase64(const char *szInANSI, int nInLen,  int *nOutLen);
Email::~Email()
{
    if( smtpserver !=  SMTPSERVER)
        free(smtpserver);
    if( myaccount!=    MYACCOUNT) 
        free(myaccount);
    if( mypasswd!=    MYPASSWD )
        free(mypasswd);
}

Email::Email()
{
    smtpserver=SMTPSERVER;
    smtpsendaddr="djx_zh@sina.com";
    myaccount=MYACCOUNT;
    mypasswd=MYPASSWD ;
}

void Email::setAuth(char* user, char* pass){
    char* p;
    p = strchr(user, '@') +1;
    smtpserver=strdup(p);
    myaccount=strdup(user);
    mypasswd=strdup(pass);
}

void Email::setAuth(char* server, char* user, char* pass){
    smtpserver=strdup(server);
    myaccount=strdup(user);
    mypasswd=strdup(pass);
}

int Email::mailfrom(char *sender,char * passwd)
{
    myaccount=sender;
    mypasswd=passwd;
}

int Email::mailserver(char *server)
{
    smtpserver=server;
}

int Email::mailto(char *mailtoaddr,char *subject,char *data)
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
    int retry = 0;
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
    msg[MAILFROM]=smtpmailfrombuf;//"mail from:<dzh@ncic.ac.cn>\n";

    smtpmailtobuf=(char *)malloc(15+strlen(mailtoaddr));
    sprintf(smtpmailtobuf,"rcpt to:<%s>\n",mailtoaddr);
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
    if(strncmp(buff,"220",3)!=0)
        return (-1);
#ifdef DZHDEB
    printf("connect:\n");
    printf("return buff:\n|%s|\n",buff);
#endif

resend1:
    retry ++;
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
            return -1;
            break;
        }
        //sleep(1);we dont have to use it,because recv() can 
        //choke itself until it received data
        if((numbytes=recv(sockfd,buff,SIZE,0))==-1)
        {
            perror("recv error");
            return -1;
            break;
        } 
        strncpy(tmp,buff,3);
#ifdef DZHDEB
        printf("/**********/\ncommand:%s\n",msg[i]);
        printf("return buff:\n|%s|\n",buff);
        printf("should return:%s\n",n_return[i]);
#endif
        if(strncmp(buff,n_return[i],3)==0) 
            i++;
        else
        {
            if((strncmp(buff,"220",3) ==0)&&(retry <6)) 
                goto resend1;
            printf("server return err:%s\n",tmp);
            return -1;
            break;
        }
    }//endwhile


    //send quit to end mail connection
    if((numbytes=send(sockfd,msg[SMTPQUIT],strlen(msg[SMTPQUIT]),0))==-1)
    {
        perror("send error");
        return(-1);
    }

    //for(i=0;i<SMTPQUIT;i++)
    //free(msg[i          ]);
    free(msg[SMTPEHLO]);
    free(msg[SMTPAUTH]);
    free(msg[SMTPUSER]);
    free(msg[SMTPPASS]);
    free(msg[MAILFROM]);
    free(msg[SMTPRCPT]);
    free(msg[SMTPDATA]);
    free(msg[SMTPCONT]);
    free(msg[SMTPQUIT]);
    close(sockfd);
    return (0);
}

int Email::mailto(char *mailtoaddr,char *subject,char *data,char* fn)
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
    int retry = 0;
    hellobuf=(char *)malloc(10+strlen(smtpserver));
    sprintf(hellobuf,"ehlo %s\n",smtpserver);
    msg[SMTPEHLO]=hellobuf;//"ehlo ncic.ac.cn\n";
    msg[SMTPAUTH]=(char *)malloc(12);//"auth login\n";
    sprintf(msg[SMTPAUTH],"auth login\n");
    usernamebuf=base64str(myaccount);
    strcat(usernamebuf,"\n");
    msg[SMTPUSER]=usernamebuf;

    passwdbuf=base64str(mypasswd);
    strcat(passwdbuf,"\n");
    msg[SMTPPASS]=passwdbuf;

    smtpmailfrombuf=(char *)malloc(15+strlen(smtpsendaddr));
    sprintf(smtpmailfrombuf,"mail from:<%s>\n",smtpsendaddr);
    msg[MAILFROM]=smtpmailfrombuf;//"mail from:<dzh@ncic.ac.cn>\n";

    smtpmailtobuf=(char *)malloc(15+strlen(mailtoaddr));
    sprintf(smtpmailtobuf,"rcpt to:<%s>\n",mailtoaddr);
    msg[SMTPRCPT]=smtpmailtobuf;//"rcpt to:<djx.zhenghua@gmail.com>\n";


    msg[SMTPDATA]=(char *)malloc(6);//"data\n";
    sprintf(msg[SMTPDATA],"data\n");

    smtpdatabuf=(char *)malloc(strlen(data)+512);
    sprintf(smtpdatabuf,"%s%s\n\n",mimeTextHeader,data);
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
    if(strncmp(buff,n_return[i],3)==0) 
        return (-1);
#ifdef DZHDEB
    printf("connect:\n");
    printf("return buff:\n|%s|\n",buff);
#endif
resend2:
    retry ++;
    i=SMTPEHLO;
    while(i<SMTPCONT) {
        if(msg[i]==NULL)
        {
            i++;
            continue;
        }
        if((numbytes=send(sockfd,msg[i],strlen(msg[i]),0))==-1)
        {
            perror("send error");
            return -1;
            break;
        }
        //sleep(1);we dont have to use it,because recv() can 
        //choke itself until it received data
        if((numbytes=recv(sockfd,buff,SIZE,0))==-1)
        {
            perror("recv error");
            return -1;
            break;
        } 
        strncpy(tmp,buff,3);
#ifdef DZHDEB
        printf("/**********/\ncommand:%s\n",msg[i]);
        printf("return buff:\n|%s|\n",buff);
        printf("should return:%s\n",n_return[i]);
#endif
        if(strncmp(buff,n_return[i],3)==0) 
            i++;
        else
        {
            if((strncmp(buff,"220",3) ==0)&&(retry <6)) 
                goto resend2;
            printf("server return err:%s\n",tmp);
            return -1;
            break;
        }
    }//endwhile

    // send content
    {
        char mimeHeader[1024];
        char attachHeader[1024];
        char *fileContent=0;
        char *filebase64=0;
        int fsize=-1;
        int attachBase64Size;
        extern        char * readFile(char *fn,int *fsize);
        sprintf(mimeHeader,"FROM:%s\nTO:%s\nSUBJECT:%s\nDate:2008-05-08\nMIME-Version: 1.0\nContent-type: multipart/mixed; boundary=\"#BOUNDARY#\"\n\n",smtpsendaddr,mailtoaddr,subject);
        sprintf(attachHeader,ATT_HEADER_TEMPLATE,getBaseFilename(fn),getBaseFilename(fn));
        fileContent = readFile(fn,&fsize);   
        debprintf("attach Size:%s %d\n", fn ,fsize);
        //filebase64 = base64str(fileContent,fsize,&attachBase64Size);
        if(fsize>0)
            filebase64 = ANSIToBase64(fileContent,fsize,&attachBase64Size);
        debprintf("send mimeHeader\n");
        if((numbytes=send(sockfd,mimeHeader,attachBase64Size,0))==-1)
        {
            perror("send mimeHeader error");
            return -1;
        }
        debprintf("send smtpdatabuf \n");
        if((numbytes=send(sockfd,smtpdatabuf,strlen(smtpdatabuf),0))==-1)
        {
            perror("send smtpdatabuf error");
            return -1;
        }
        if(fsize>0){
            if((numbytes=send(sockfd,attachHeader,strlen(attachHeader),0))==-1)
            {
                perror("send attachHeader error");
                return -1;
            }
            debprintf("send attach file len:%d \n",  attachBase64Size);
            if((numbytes=send(sockfd,filebase64, attachBase64Size/*  strlen(filebase64)*/,0))==-1)
            {
                perror("send filebase64 error");
                return -1;
            }
        }
        if((numbytes=send(sockfd,"\n.\n",strlen("\n.\n"),0))==-1)
        {
            perror("send end error");
            return -1;
        }
        delete (fileContent);
        delete (filebase64);
        if((numbytes=recv(sockfd,buff,SIZE,0))==-1)
        {
            perror("recv error");
            return -1;
        } 
    }

    //send quit to end mail connection
    if((numbytes=send(sockfd,msg[SMTPQUIT],strlen(msg[SMTPQUIT]),0))==-1)
    {
        perror("send error");
        return(-1);
    }
    debprintf("send OK\n");
    free(msg[SMTPEHLO]);
    free(msg[SMTPAUTH]);
    free(msg[SMTPUSER]);
    free(msg[SMTPPASS]);
    free(msg[MAILFROM]);
    free(msg[SMTPRCPT]);
    free(msg[SMTPDATA]);
    free(msg[SMTPCONT]);
    free(msg[SMTPQUIT]);
    close(sockfd);
    return (0);
}

/*-------------------------
  base64 encode function
  -------------------------*/
char * base64str(const char *instr,int instrSize,int *outSize)
{ 
    char * table="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    int instr_len=0,i=0,j=0,pad=0; 
    unsigned char buf1[4]="",buf2[4]=""; 
    char *outstr;
    int *pintbuf2 =(int*) buf2;
    int *poutint ;
    if(instrSize)
        instr_len = instrSize;
    else
        instr_len=strlen(instr);
    pad=instr_len%3;
    outstr=(char *)malloc((instr_len/3)*4+6);
    poutint = (int*)outstr;
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
        *poutint++ = *pintbuf2;
        //strncat(outstr,(char *)buf2,4);
    }
    *(char*)poutint = 0;
    if(outSize){
        *outSize = (int) ((char*)poutint - outstr);
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

char * ANSIToBase64(const char *szInANSI, int nInLen,  int *nOutLen)
{
    //Input Parameter validation
    //if ((szInANSI == NULL) || (nInLen == 0) || (szOutBase64 == NULL) || (nOutLen == 0))
    //return 0;
    //if (nOutLen < (nInLen*4/3 + 1 + nInLen*4/3/BASE64_MAXLINE*2 + 1 + 4))
    //return 0;
    char *szOutBase64;
    szOutBase64 = new char[2*nInLen];
    //Set up the parameters prior to the main encoding loop
    int nInPos  = 0;
    int nOutPos = 0;
    int nLineLen = 0;
    int c1, c2, c3;
    int i;

    // Get three characters at a time from the input buffer and encode them
    for (i=0; i<nInLen/3; ++i)
    {
        //Get the next 2 characters
        c1 = szInANSI[nInPos++] & 0xFF;
        c2 = szInANSI[nInPos++] & 0xFF;
        c3 = szInANSI[nInPos++] & 0xFF;

        //Encode into the 4 6 bit characters
        szOutBase64[nOutPos++] = BASE64_TAB[c1 >> 2];
        szOutBase64[nOutPos++] = BASE64_TAB[((c1 << 4) | (c2 >> 4)) & 0x3F];
        szOutBase64[nOutPos++] = BASE64_TAB[((c2 << 2) | (c3 >> 6)) & 0x3F];
        szOutBase64[nOutPos++] = BASE64_TAB[c3 & 0x3F];
        nLineLen += 4;

        //Handle the case where we have gone over the max line boundary
        if (nLineLen > BASE64_MAXLINE - 4)
        {
            szOutBase64[nOutPos++] = EOL[0];
            szOutBase64[nOutPos++] = EOL[1];
            nLineLen = 0;
        }
    }

    // Encode the remaining one or two characters in the input buffer
    switch (nInLen % 3)
    {
        case 0:
            {
                szOutBase64[nOutPos++] = EOL[0];
                szOutBase64[nOutPos++] = EOL[1];
                break;
            }
        case 1:
            {
                c1 = szInANSI[nInPos] & 0xFF;
                szOutBase64[nOutPos++] = BASE64_TAB[(c1 & 0xFC) >> 2];
                szOutBase64[nOutPos++] = BASE64_TAB[((c1 & 0x03) << 4)];
                szOutBase64[nOutPos++] = '=';
                szOutBase64[nOutPos++] = '=';
                szOutBase64[nOutPos++] = EOL[0];
                szOutBase64[nOutPos++] = EOL[1];
                break;
            }
        case 2:
            {
                c1 = szInANSI[nInPos++] & 0xFF;
                c2 = szInANSI[nInPos] & 0xFF;
                szOutBase64[nOutPos++] = BASE64_TAB[(c1 & 0xFC) >> 2];
                szOutBase64[nOutPos++] = BASE64_TAB[((c1 & 0x03) << 4) | ((c2 & 0xF0) >> 4)];
                szOutBase64[nOutPos++] = BASE64_TAB[((c2 & 0x0F) << 2)];
                szOutBase64[nOutPos++] = '=';
                szOutBase64[nOutPos++] = EOL[0];
                szOutBase64[nOutPos++] = EOL[1];
                break;
            }
        default:
            {
                return 0;
            }
    }

    szOutBase64[nOutPos] = 0;
    *nOutLen = nOutPos; 
    return szOutBase64;
}

char* getBaseFilename(const char * fn){
    const char* p = fn + strlen(fn); 
    while( (p >= fn) && (*p != '/') &&(*p != '\\')) p--;
    return (char*)p;
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
