#include <stdio.h>
#include <stdlib.h>
#include "lib.h"
int main(int argc, char *argv[]){
    char *cont;
    char *email="djx.zhenghua@gmail.com";
    char * data;
    char *subj="ip";
    cont=httpsock("http://www.ip138.com/ips.asp",NULL);
    mailto(email,cont,subj);
    //printf("%s",cont);
}
