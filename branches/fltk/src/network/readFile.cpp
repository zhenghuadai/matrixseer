/*
 * =====================================================================================
 *
 *       Filename:  readFile.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2008年05月09日 10时29分32秒
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
#define LINUX
char * readFile(char *fn,int *fsize)
{
    FILE* fp;
    char c;
    char *content ;
    int size=-1;
#ifdef LINUX
    struct stat fsc;
    fsc.st_size = -1;
    stat(fn, &fsc);
    *fsize = size = fsc.st_size;
    printf("size:%d\n",size);
#endif
    if(size<=0) {
        //printf("size err\n");
        return NULL;
    }
    content = new char[size+1];
    fp = fopen(fn,"r");
    fread(content,1,size,fp);
    fclose(fp);
    content[size] = 0;
    return content;

}
#ifdef testReadfile
#include "string.h"
int main(int argc,char** argv)
{
    char * fn = argv[1];
    char * fc;
    int size;
    FILE *fp;
    char bufn[32];
    //strcpy(bufn,"djx_zh.sina.com.uidl");
    sprintf(bufn,"%s","djx_zh.sina.com.uidl");
    //fn = strdup(fn);
    fc= readFile(bufn,&size);
    //fc= readFile(fn,&size);
    //fp = fopen("djx_zh.sina.com.uidl","w");
    //fwrite(fc,1,size,fp);
    //fclose(fp);
    printf("%s",fc);
    }
    #endif
    
