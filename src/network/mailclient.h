#ifndef __MAIL_CLIENT_HEADER_
#define __MAIL_CLIENT_HEADER_
#include <stdlib.h>
#include <stdio.h>
#include "mail_t.h"
template <typename T>
inline void Mfree(T * p){
    if(p !=NULL) free(p);
    p = NULL;
}
class uidl_t
{
    char type;
    char t1;
    char t2;
    char t3;
    int a[2];
    uidl_t(){}
    uidl_t(char* u){set(u);}
    void set(char* u){}
};

class mailaccount
{
    private:
        char user[32];
        char passwd[16];
        char pop3server[32];
        char* accountpath;
        char* uidlfn;
        char* configfn;
        char* datafn;
        int sockfd;

        int *newmailid;
        int newm ;

        int allDownloadMail;

        int currentMailNum;
        uidl_t *uidlArray;
        void storeMail(char* m);
        void storeUidl();
        void loadUidl();
        void init()
        {
            newm =0;
            uidlfn = NULL;
            datafn = NULL;
            accountpath = NULL;
            newmailid = NULL ;
        }
        void saveAbstract(mail_t *mc); 
    public:
        mailaccount () { init();}
        mailaccount(int id){
            init();
            if(id == 2){
                strcpy(user,"djx_zh");
                strcpy(passwd,"djx_zh010");
                strcpy(pop3server,"pop3.sina.com.cn");
                accountpath = strdup("/home/dzh/.dmail/djx_zh.sina.com/");
                uidlfn=strdup("/home/dzh/.dmail/djx_zh.sina.com/uidl");
                configfn = strdup("/home/dzh/.dmail/djx_zh.sina.com/config");
                datafn = strdup("/home/dzh/.dmail/djx_zh.sina.com/data");

            }
            newm =0;
        }
        ~mailaccount()
        {
            Mfree(uidlfn);
            Mfree(accountpath);
            Mfree(newmailid);
        }
        void  printconfig(){
            printf("user: %s\n",user);
            printf("pass: %s\n",passwd);
            printf("server: %s\n",pop3server);
            printf("mails:%d\n",currentMailNum);
        }
        void loadconfig(char* accountHome){
            accountpath = strdup(accountHome);
            char buf[128],*bufp;
            sprintf(buf,"%s/uidl",accountpath);
            uidlfn = strdup(buf);
            sprintf(buf,"%s/config",accountpath);
            configfn = strdup(buf);
            sprintf(buf,"%s/data",accountpath);
            datafn =  strdup(buf);

            /* open configure file to GET:
             * usr; passwd; pop3server
             */
            FILE* fp = fopen(configfn,"r");
            if(fp ==NULL)
            {
            }
            fgets(buf,127,fp);
            currentMailNum = *(int*) buf;

            fgets(buf,127,fp);// get user
            buf[strlen(buf)-1] = 0; // not safe
            bufp = buf; 
            while(*bufp != ' ') bufp ++; 
            bufp ++;
            strcpy(user , bufp);

            fgets(buf,127,fp);// get pass 
            buf[strlen(buf)-1] = 0; // not safe
            bufp = buf; 
            while(*bufp != ' ') bufp ++; 
            bufp ++;
            strcpy(passwd ,bufp);

            fgets(buf,127,fp);// get server 
            buf[strlen(buf)-1] = 0; // not safe
            bufp = buf; 
            while(*bufp != ' ') bufp ++; 
            bufp ++;
            strcpy(pop3server,bufp);
            printconfig();
        };
    private:
        void storeconfig(){
            //char path[128];
            //sprintf(path,"%sconfig",accountpath);
            FILE* fp = fopen(configfn,"w");
            if(fp ==NULL)
            {
            }
            fwrite(&currentMailNum,4,1,fp);
            fprintf(fp,"\n");
            fprintf(fp,"user %s\n",user);
            fprintf(fp,"pass %s\n",passwd);
            fprintf(fp,"server %s\n",pop3server);
            fclose(fp);
        };
        void saveCurrentMailNum(){
            FILE* fp = fopen(configfn,"r+");
            fwrite(&currentMailNum,4,1,fp);
            fclose(fp);
        };
        void setuser(char* u){strcpy(user,u);}
        void setpass(char* p){strcpy(passwd,p);}
        void setServer(char* s){strcpy(pop3server,s);}
        int createfile(char* mroot){
            char path[128];
            sprintf(path,"%s/%s.%s/",mroot,user,pop3server);
            if(mkdir(path, 0700) == -1){
                perror("Couldn't create the directory.\n");
                return 1;
            }
            accountpath = strdup(path);
            //sprintf(path + strlen(path),"/%s.%s.uidl",user,pop3server);
            sprintf(path + strlen(path),"/uidl");
            FILE* fp = fopen(path,"w");
            if(fp ==NULL){
                printf("could not create uidl file\n");
                return 1;
            }
            fclose(fp);
            uidlfn = strdup ( path);
            sprintf(path,"%sconfig",accountpath);
            configfn = strdup(path);
            currentMailNum = 0;
        }
    public:
        void create();
        void create(char* dmpath);
        int isNew(uidl_t* u){};
        int check();
        void listMail();
        void close(){};
};
#endif
