#include <stdio.h>
#include <stdlib.h>
#include "lib.h"
int main(int argc,char *argv[])
{
//char *email="djx.zhenghua@gmail.com";
char *email="djx_zh@sina.com";
char * data="pic";
char *subj="good";
char *fn = argv[1];
int ret ;
if(argc == 2){
    fn = argv[1];
}
else if(argc == 3)
{
    email = argv[1];
    fn = argv[2];
}
Email myemail;
ret = myemail.mailto2(email,data,subj,fn);
printf("send %s %s\n",fn,ret?"fail":"ok");
}
