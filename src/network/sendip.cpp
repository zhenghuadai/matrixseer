#include <stdio.h>
#include <stdlib.h>
#include "lib.h"
#include "string.h"

int main(int argc, char *argv[]){
char *cont;
char *email="djx.zhenghua@gmail.com";
char * data;
char *subj="ip";
char* preIP = "[";
char *ip;
char buffIP[32];
int i=0;
FILE *fp;
char oldIP[32];
Email mailServer;
cont=httpsock("http://www.ip138.com/ips.asp",NULL);

ip = cont;
while((ip = strstr(ip,preIP)) !=NULL){
    ip++;
    if((ip[0]>='0')&&(ip[0]<='9')) break;
}
while((*ip) && (*ip != ']') && (i<30)){
    buffIP[i++] = *ip++;
}
buffIP[i] =0;

fp = fopen("/home/dzh/tmp/IP","r");
if(fp == NULL) return 1;
fgets(oldIP,16,fp);
fclose(fp);
if( strcmp(buffIP,oldIP) !=0){
    mailServer.mailto(email,buffIP,subj);
    //if(ip)printf("%s",buffIP);
    fp = fopen("/home/dzh/tmp/IP","w");
    fputs(buffIP,fp);  
    fclose(fp);
    //fprintf(fp,"%s",buffIP);
}
//mailServer.mailto(email,buffIP,subj);
//if(ip)printf("%s",buffIP);
//printf("%s",cont);
//printf("%s",oldIP);
}
