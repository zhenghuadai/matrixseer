#include <stdio.h>
#include <stdlib.h>
#include "lib.h"
void testFile(int argc,char *argv[])
{
    char *cont;
    if (argc < 2) {
        fprintf(stderr,"Please enter the server's hostname!\n");
        exit(1);
    }
    printf("%s\n",argv[1]);
    http_t ht;
    //httpget(argv[1],&ht,NULL);
    httpget(argv[1],&ht,200,6000,NULL);
    printf("%s\n",ht.head);
    //printf("%s",ht.content);
    printf("The size :%d \n", ht.length);
    FILE* fp;
    fp = fopen("tmp.gif","w");
        for(int i=0;i<ht.length;i++)
        fprintf(fp,"%c",ht.content[i]);
        fclose(fp);
    //cont=httpsock(argv[1],NULL);
    //printf("%s",cont);
}
int testhtml(int argc, char *argv[]){
    char *cont;
    if (argc < 2) {
        fprintf(stderr,"Please enter the server's hostname!\n");
        exit(1);
    }
    printf("%s\n",argv[1]);
    http_t ht;
    //httpget(argv[1],&ht,NULL);
    //printf("%s",ht.content);
}
int main(int argc, char *argv[]){
    //testFile(argc,argv);
    httpDownLoad(argv[1],argv[2]);
}
