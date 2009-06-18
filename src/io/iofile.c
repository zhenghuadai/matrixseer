#include "stdio.h"
#include "stdlib.h"
#define __LINUX__
#ifdef __LINUX__
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
int isfile(char *fn)
{
    int i;
    i = access(fn,F_OK) ;
    return !i;
}

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

char * getsfromfile(char *fn)
{
    FILE * fp;
    int filesize = 0;
    struct stat statf;
    char *rbuf;
    if(stat(fn,&statf)== -1){
        return 0;
    }
    filesize = statf.st_size + 1;
    rbuf = (char *)malloc(filesize);
    if(rbuf == NULL) return 0;
    fp = fopen(fn,"r");
    if(fp ==NULL) return 0;
    fread(rbuf,1,filesize,fp);
    return rbuf;
}

int is_fileexist(char *comm)
{
  char *path,*p;
  int i;
  char buffer[100];
  i=0;
  path=getenv("PATH");
  strcat(path,"./:");
  p=path;
  while(*p!='\0')
    {
      if(*p!=':')
	buffer[i++]=*p;
      else
	{
	  buffer[i++]='/';
	  buffer[i]='\0';
	  strcat(buffer,(const char *)comm);
	  if(access(buffer,F_OK)==0)
	    {
	      //  printf("%s\n",buffer);
	     return 0;
	    }
	  else
	    i=0;
	}
      p++;
    }
  return -1;
}
#endif
