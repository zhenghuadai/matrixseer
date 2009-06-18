#include "urlManager.h"
#include "downLoader.h"
#include <sys/time.h>
#include <signal.h>
#include <pthread.h>
#include "saver.h"
int running = RUNNING;
downLoader mDL;
urlManager urler;
urlManager *purler;
downLoader *pDL;
void timeHandler (int signo)
{
    static int last_i=0;
    unsigned int i, j;
    if(signo != SIGALRM) return ;
    urler.createURL();
    urler.sendURL(&mDL);
    //fprintf(stdout,"send\n");
    signal(SIGALRM,timeHandler);
}

int setTimer ()
{
    struct itimerval value2,ovalue;

    signal(SIGALRM,timeHandler);
    value2.it_value.tv_sec = 1;
    value2.it_value.tv_usec = 100000;
    value2.it_interval.tv_sec = 10;
    value2.it_interval.tv_usec = 100000;
    setitimer(ITIMER_REAL, &value2, &ovalue);
}

void*  readFromDowner(void * arg)
{
    webpage* wp;
    saver wr;
    while (running == RUNNING){
        wp= mDL.popB();
        if(wp !=NULL){
            //debprintf("%s\n",wp->content);
            //debprintf("%s\n%s\n",wp->url.url,smthGetContentFromsTx(wp->content));
            //debprintf("***************************\n");
            //wr.print("%s\n",wp->content);
            char* content = smthGetContentFromsTx(wp->content);
            wr.print("%s\n%s\n",wp->url.url,content);
            wr.print("\n***************************\n\n");
            free(content);
            //fprintf(stdout,"%s %d\n",wp->url.url, wp->url.type);
            //fflush(stdout);
            if(strstr(wp->content,"发生错误") !=NULL)
                urler.errforwardURL(wp->url.type);
            else
                urler.forwardURL(wp->url.type);
            delete wp;
        }
    }
}

inline void shutdown()
{
    running = SHUTDOWN;
    urler.savePoint();
    pDL -> quit();
}

void shell()
{
    char buf[1024];
    while(1){
        char *pcur=buf;
        fprintf(stdout,"downer\033[32m$\033[0m");
        fgets(buf,127,stdin);
        while((*pcur == ' ')||(*pcur=='\n'))pcur++;
        if(pcur[0]==0)continue;
        if(pcur[0]=='q'){

            shutdown(); 
            break;
        }
        if(strncmp("savepoint",pcur,9) == 0){
            urler.savePoint();
        } else if(strncmp("loadpoint",pcur,9) ==0){
            urler.loadPoint();
        }
    }
}

int main(int argc,char **argv)
{
    pthread_t readthread;
    mDL.createThread();
    pDL = &mDL;
    purler = &urler;
    urler.prints();
    setTimer();
    //urler.createURL();
    //urler.sendURL(pDL);
    pthread_create(&readthread,NULL,readFromDowner,(void*)NULL);
    //pthread_join(readthread,NULL);
    //readFromDowner(&mDL);
    shell();
    mDL.terminateThread();
    exit(0);
}
