#include <stdio.h>
#include <stdlib.h>
#include "lib.h"
#include "string.h"
void help(){
    printf("sendmail --user user --passwd passwd --from from --to to --subj subj --content content --attach file\n");
    printf("\n");
}
int main(int argc, char *argv[]){

    char *user=0, *passwd=0, *from=0, *to=0, *cc=0, *subj=0, *content=0, *attachment_file=0;
    Email mailServer;
    if(argc <4){
        help();
        return 0;
    }

    for(int i=0;i<argc;i++){
        if((argv[i][0] == '-') &&(argv[i][1] == '-')){
            if(       strcmp(&argv[i][2], "user") ==0){
                user=argv[i+1];
            }else if( strcmp(&argv[i][2], "passwd") ==0){
                passwd=argv[i+1];
            }else if( strcmp(&argv[i][2], "from") ==0){
                from=argv[i+1];
            }else if( strcmp(&argv[i][2], "to") ==0){
                to=argv[i+1];
            }else if( strcmp(&argv[i][2], "subj") ==0){
                subj=argv[i+1];
            }else if( strcmp(&argv[i][2], "content") ==0){
                content=argv[i+1];
            }else if( strcmp(&argv[i][2], "attach") ==0){
                attachment_file=argv[i+1];
            }
            i++;
        }
    }

    mailServer.setAuth("smtp.sina.com.cn", user,passwd);
    mailServer.mailto(to,subj,content, attachment_file);
}
