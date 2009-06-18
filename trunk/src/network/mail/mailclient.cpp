/*
 * =====================================================================================
 *
 *       Filename:  mail.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2008年08月12日 21时47分19秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zhenghua Dai (Zhenghua Dai), djx.zhenghua@gmail.com
 *        Company:  dzh
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include  <unistd.h>
#include "mailclient.h"
#include "mail_t.h"
#if 0
#endif
//int isNew()
//{
//    
//}
struct fileHead_
{
    int type;
    size_t size;
    size_t mailPosHead;
    size_t mailPosHeadLen;
};

#include "pop3socket.cpp"
#include "readFile.cpp"
#define testErrCode \
    if(errcode != SOCKSUCESS){\
        printf("not connected\n");\
        return errcode;\
    }
void mailaccount::listMail()
{
    FILE* fp = fopen(datafn,"r");
    char buf[1024];
    int i =0;
    printf("%d\n",i++);
    while(fgets(buf,1023,fp) != NULL){
        if(buf[0] == '\n'){
            printf("%d\n",i++);
        }
        else
        printf("%s",buf);
    }
    fclose(fp);
}
int mailaccount::check()
{
    newm =0;
    int errcode = pop3Connect(pop3server,user,passwd,sockfd);
    // safe call
    if(errcode != SOCKSUCESS){
        printf("not connected\n");
        return 1;
    }
    int mailNumOnServer;
    int sizeofallmail;
    errcode =  pop3Stat(sockfd,mailNumOnServer,sizeofallmail);
    testErrCode;
    newmailid = (int*)malloc(mailNumOnServer*sizeof(int));
    printf("mails :%d\n",mailNumOnServer);
    int olduidlas;
    char* olduidlarray = readFile(uidlfn,&olduidlas);
    buf_t uidlbuf;
    errcode = pop3Uidl(sockfd,uidlbuf);
    if(errcode != SOCKSUCESS){
        printf("err\n");
        return 1;
    }
    char *pstr = uidlbuf.buf;
    char *pend = uidlbuf.buf+uidlbuf.len;
    FILE *uidlfp = fopen(uidlfn,"a");
    if(uidlfp==NULL){
        printf("open %s err\n",uidlfn);
        return 0;
    }
    for(int i =1;i<= mailNumOnServer;i++){
        int mailid;
        char mailuidl[128];
        pstr = gotoCnext(pstr,'\n');
        mailid = atoi ( pstr);
        DEBE(printf("cmp mail:%d\n",mailid));
        if(i != mailid) {printf(" i != id err\n"); return 1;}
        pstr = gotoCnext(pstr,' ');
        char* poldstr= pstr;
        pstr= gotoCnext(pstr,'\r');
        memcpy(mailuidl,poldstr,pstr-poldstr-1);
        mailuidl[pstr - poldstr -1] =0;
        char* findout =NULL;
        if(olduidlarray!=NULL) findout = strstr(olduidlarray,mailuidl);
        if(findout ==NULL){
            newmailid[newm] = i;
            newm ++;
            fprintf(uidlfp,"%s\r\n",mailuidl);
            printf(" new %d %s\n",i,mailuidl);
        }
    }
    fclose(uidlfp);
    free(olduidlarray);

    // get new mail
    for(int i=0;i<newm;i++){
        buf_t mailbuf;
        errcode = pop3Retr(sockfd,newmailid[i],mailbuf);
        DEBE(printf("mailID %d\n",newmailid[i]));
        char buf[128];
        sprintf(buf,"%s/xm%d",accountpath,currentMailNum++);
        if(access(buf,R_OK) == 0){
            int last = strlen(buf);
            int secondi=0;
            while(access(buf,R_OK) == 0){
                sprintf(buf+last,"_%d",secondi++);
            }
        }
        FILE* fp= fopen(buf,"w");
        fwrite(mailbuf.buf,mailbuf.len,1,fp);
        mail_t mc;
        mc.parseAbstractFromMail(mailbuf.buf);
        saveAbstract(&mc);
        fclose(fp);
        free(mailbuf.buf);
        saveCurrentMailNum();
    }
}

void mailaccount::saveAbstract(mail_t *mc) 
{
    FILE* fp = fopen(datafn,"a");
    fprintf(fp,"From:%s\n",mc->from);
    fprintf(fp,"to:%s\n",mc->to);
    fprintf(fp,"subject:%s\n",mc->subject);
    fprintf(fp ,"date:%s\n",mc->date);
    fprintf(fp,"file:%s\n",datafn);
    fprintf(fp,"\n");
    fclose(fp);
}
void mailaccount::create(char* dmpath)
{
    char username[32];
    char pass[32];
    char accountname[32];
    char popserver[128];
    printf("username:");
    fscanf(stdin,"%s",username);

    printf("passwd:");
    fscanf(stdin,"%s",pass);

    printf("popserver:");
    fscanf(stdin,"%s",popserver);

    setuser(username);
    setpass(pass);
    setServer(popserver);
    createfile(dmpath);
    storeconfig();

    char buf[128];
    sprintf(buf,"%s/config",dmpath);
    FILE* fp = fopen(buf,"a");
    fprintf(fp,"%s.%s\n",username,popserver);
    fclose(fp);
}
#if 0
int getallAccount(char* rootpath,char** accountName,int& accountNum)
{
    char buf[128];
    sprintf(buf,"%s/config",rootpath);
    FILE* fp = fopen(buf,"r");
    char* childp = buf + strlen(rootpath)  +1;
    while(fgets(childp,127-strlen(rootpath)-1,fp) != NULL){
        buf[strlen(buf) -1] =0;
        accountName [ accountNum] = strdup(buf);
        accountNum++;
    }
}

#define printPRO \
    fprintf(stdout,"$ ");
#define CHECKOP 0
#define LISTOP 1
#define QUITOP 2
void goIntoAccount(mailaccount *A)
{
    while(1){
        printf("%d check\n",CHECKOP);
        printf("%d list\n",LISTOP);
        printf("%d quit\n",QUITOP);
        printPRO;
        int opcode = fgetc(stdin) ;
        while(opcode == '\n') opcode = fgetc(stdin);
        opcode -= '0';
        fflush(stdin);
        if(opcode == CHECKOP){
            A -> check();
        }else if (opcode == LISTOP){
            A -> listMail();
        }else if(opcode == QUITOP){
            A -> close();
            return;
        }
    }
}
char* dmailpath =NULL;
int accountNum;
char* accountName[10];
int main(int argc,char** argv)
{
    char* home = getenv("HOME");

    mailaccount* currentAccount= new mailaccount;
    dmailpath = (char*) malloc( strlen(home) + 10);
    sprintf(dmailpath,"%s/.dmail/",home);

    getallAccount(dmailpath,accountName,accountNum);
    while(1){
        for(int i=0;i< accountNum;i++)
            printf("%d open %s\n",i,accountName[i]);
        printf("%d create new account\n",accountNum);
        printf("%d quit\n",accountNum+1);
        printPRO;
        int opcode = fgetc(stdin) ;
        while(opcode == '\n') opcode = fgetc(stdin);
        opcode -= '0';
        fflush(stdin);
        if(opcode == accountNum){
            currentAccount->create(dmailpath);
        }
        else if(opcode < accountNum){
            currentAccount->loadconfig(accountName[opcode]); 
        }
        else if(opcode == accountNum + 1){// quit
            return 1;
        }
        else{
            printf("err opcode\n");
            return 1;
        }
        goIntoAccount(currentAccount);
    }
}
#endif
