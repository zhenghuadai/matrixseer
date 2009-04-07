/*
 * =====================================================================================
 *
 *       Filename:  mail_t.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2008年08月30日 13时46分15秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zhenghua Dai (Zhenghua Dai), djx.zhenghua@gmail.com
 *        Company:  dzh
 *
 * =====================================================================================
 */
#include "stdio.h"
#include "stdlib.h"
#include "mail_t.h"
#include "string.h"
int mail_t::parseAbstractFromFile(char *fn)
{
    
}

void cponeline(char* dst,char* src)
{
    while((*src)&&(*src != '\n') && (*src != '\r'))
        *dst++ = *src++;
}

size_t getLenofOneline(char* src)
{
    int len;
    while((*src)&&(*src != '\n') && (*src != '\r'))
        {
            len ++;
            src++;
        }
    return len;
}

char* getOneLine(char* src)
{
    int len ; 
    len=getLenofOneline(src);
    char* dst;
    dst = (char* )malloc(len+1);
    cponeline(dst,src);
    dst[len] =0;
    return dst;
}

int  mail_t::parseAbstractFromMail(char *email)
{
    if(email==NULL) return 1; 
    char* p;
    p = strstr(email,"From:") ;
    if(p !=NULL){
        p += 5;
        from = getOneLine(p); 
    }

    p = strstr(email,"To:") ;
    if(p !=NULL){
        p += 3;
        to = getOneLine(p);
    }

    p = strstr(email,"Subject:") ;
    if(p !=NULL){
        p += 8;
        subject = getOneLine(p);
    }

    p = strstr(email,"Date:") ;
    if(p !=NULL){
        p += 5;
        date = getOneLine(p);
    }
    return 0;
}
#ifdef MAILT_MAIN
#include "readFile.cpp"
int main(int argc, char** argv)
{
    char* fn;
    fn = argv[1];

    int flen;
    char* mail = readFile(fn,&flen);
    mail_t mc;
    mc.parseAbstractFromMail(mail);
    FILE* fp = fopen("data","a");
    fprintf(fp,"From:%s\n",mc.from);
    fprintf(fp,"to:%s\n",mc.to);
    fprintf(fp,"subject:%s\n",mc.subject);
    fprintf(fp ,"date:%s\n",mc.date);
    fprintf(fp,"file:%s\n",fn);
    fprintf(fp,"\n");
}
#endif
