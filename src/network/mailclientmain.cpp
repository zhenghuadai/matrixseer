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
#include "mail_t.h"
#include "mailclient.h"
#if 1
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
