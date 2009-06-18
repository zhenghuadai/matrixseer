/*
 * =====================================================================================
 *
 *       Filename:  webpage.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  05/05/2008 09:36:45 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zhenghua Dai (Zhenghua Dai), djx.zhenghua@gamil.com
 *        Company:  dzh
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include "downLoader.h"
static void noop(int * noop_value)
{
	int lv = *noop_value;
	for( int x = 0; x < 50; x++ ) {
		__asm__ (
				"mov %0,%%eax;\
				inc %%eax;\
				mov %%eax,%0;\
				":"=r"(lv)
				:"0"(lv)
				:"%eax"
				);
	}
	*noop_value = lv;
}

downLoader::downLoader()
{
	urlReader = 0;
	urlWriter = 0;
	webPReader =0 ;
	webPwriter = 0;
    threadNum = 0;
	fullSize = POOLSIZE;
    state = RUNNING;
    logfp = fopen("logget","a");
    sem_init(&(semURLPoolEmpty),0,0);
    sem_init(&(semWebPoolEmpty),0,0);
    for(int i=0;i<4;i++)
        sem_init(&(pv[i]),0,1);
    memset(urlPool,0,POOLSIZE *sizeof(url_t));    
}

downLoader::~downLoader()
{
    fclose(logfp);
    //pthread_join(runThread,NULL);
}

int downLoader::pushURL(url_t& url)
{
    if(isURLPoolFull()) return -1;
    int lastIsEmpty = 0;
    if(isURLPoolEmpty()) 
        lastIsEmpty = 1;
    P(0);
    urlPool[urlWriter].url = strdup(url.url);
    urlPool[urlWriter].type = url.type;
    urlWriter =(urlWriter+1)%fullSize;
    V(0);
    if(lastIsEmpty)sem_post(&semURLPoolEmpty);
    return 0;
}

url_t downLoader::popURL()
{
    if(isURLPoolEmpty())
            sem_wait(&semURLPoolEmpty);
    if(isURLPoolEmpty())
        //return (char*)  NULL;
        return url_t(NULL,-1);
        P(1);
    int tmpReader = urlReader;
    urlReader = (urlReader + 1) % fullSize;
    V(1);
    debprintf("get URL %s\n",urlPool[tmpReader].url);
    return 	urlPool[tmpReader];
}

int downLoader::pushWebPage(url_t url,char *wp)
{
    if(isWPPoolFull()) return -1;
    int lastIsEmpty = 0;
    if(isWPPoolEmpty()) lastIsEmpty = 1; 
    P(2);
    int tmpWriter = webPwriter;
    webPwriter = (webPwriter + 1) % fullSize;
    V(2);
    webPagePool[tmpWriter].url = url;
    webPagePool[tmpWriter].content = wp;
    if(lastIsEmpty) sem_post(&semWebPoolEmpty);
    return 0;
}

webpage* downLoader::popWebPage()
{
    if(isWPPoolEmpty()) return (webpage*)NULL;
    P(3);
    int tmpReader = webPReader;
    webPReader = (webPReader + 1) % fullSize;
    V(3);
    webpage * tmpwp = new webpage;
    tmpwp->url = webPagePool[tmpReader].url;
    tmpwp->content = webPagePool[tmpReader].content;
    debprintf("%s\n",tmpwp->url.url);
    if(tmpwp->content ==(char*) 0x1)
    tmpwp->content = NULL;
    //fprintf(stdout," 0x1");
    //fflush(stdout);
    webPagePool[tmpReader].url.url = NULL;
    webPagePool[tmpReader].content = NULL;
    return tmpwp;
}

webpage* downLoader::popWebPageB()
{
    if(isWPPoolEmpty()) 
        sem_wait(&semWebPoolEmpty);
    if(isWPPoolEmpty()){
        debprintf("err \n");
        return NULL;
    }
    return popWebPage();
}

void downLoader::logGet( char *urlp)
{
    if(logfp){
        fprintf(logfp,"%s\n",urlp);
        fflush(logfp);
    }
}
void downLoader::run( int tid)
{
    url_t	tmpurl;
    //while((tmpurl= popURL())!=NULL)
    while(state == RUNNING){
        tmpurl = popURL();
        /*
        while(tmpurl.url == NULL) {
            int waitTime = 0;
            while(waitTime <10){
                int x;
                waitTime ++;
                tmpurl = popURL();
                if (tmpurl.url !=NULL) 
                    goto GetURL;
                else
                    noop(&x);
            }
            tmpurl = popURL();
        }
        */
GetURL:
        if(tmpurl.url != NULL)	{
            char *html = httpget(tmpurl.url);
            debprintf("receive %s\n",tmpurl.url);
    if(html == (char*)0x1) html = NULL; 
    //fprintf(stdout," httpget 0x1");
    //fflush(stdout);
            logGet(tmpurl.url);
            pushWebPage(tmpurl,html);
        }
    }
}

typedef struct{
    downLoader* downer;
    int threadID;
}threadArg_t;


void* runner(void* arg)
{
    threadArg_t *Targ = (threadArg_t*) arg;
    int threadID = Targ -> threadID;
    //debprintf("runner arg %0x\n",Targ->downer);
    Targ -> downer -> run(threadID);
    //mDL.run();
    return (void*) 0;
}

threadArg_t Targ;
void downLoader::createThread()
{

    Targ.downer = this;
    //debprintf("this %0x\n",Targ.downer);
    Targ.threadID = threadNum;
    threadNum++;
    pthread_create(&(runThread[Targ.threadID]) ,NULL,runner,(void*)&Targ); 	
}

int downLoader::terminateThread()
{
    for(int i=0;i<threadNum;i++)
        pthread_join(runThread[i],NULL);
}

void  downLoader::quit()
{
    state = SHUTDOWN;
    sem_post(&semWebPoolEmpty);
    sem_post(&semURLPoolEmpty);
}
