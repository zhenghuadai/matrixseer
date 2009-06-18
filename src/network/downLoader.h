/*
 * =====================================================================================
 *
 *       Filename:  downLoader.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2008年05月07日 20时28分15秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zhenghua Dai (Zhenghua Dai), djx.zhenghua@gmail.com
 *        Company:  dzh
 *
 * =====================================================================================
 */
#ifndef __DOWNLOADER__HEADER__
#define __DOWNLOADER__HEADER__
#define POOLSIZE 1024
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include "netlib.h"
#include "url.h"
#include "debprintf.h"
#define RUNNING 1
#define SHUTDOWN 0

typedef struct _webpage
{
    url_t url;
    char * content;
    int length;
    _webpage(){url.url = NULL;content =NULL;}
    ~_webpage(){delete content;delete url.url;};
}webpage;

class downLoader 
{
    public: 
        downLoader();
        ~downLoader();
        int push(url_t& url){return pushURL(url);};
        int push(char* url){url_t tmp(url,0);return pushURL(tmp);};
        webpage* pop(){return popWebPage();};
        webpage* popB(){return popWebPageB();};
        webpage* get(){return popWebPage();};
        webpage* getB(){return popWebPageB();};
        void run(int tid);
        void createThread();
        int getThreadNum(){return threadNum;}
        int terminateThread();
        int isURLPoolFull(){int tmpW = (urlWriter+1)%fullSize; return (tmpW == urlReader);};
        int isURLPoolEmpty(){return (urlReader == urlWriter);};
        int isWPPoolFull(){int tmpW = (webPwriter+1)%fullSize; return (tmpW == webPReader);};
        int isWPPoolEmpty(){return (webPReader == webPwriter);};
        void waitWPpool(){if(isWPPoolEmpty()) sem_wait(&semWebPoolEmpty);}
        pthread_t runThread[64];
        sem_t semURLPoolEmpty;
        sem_t semWebPoolEmpty;
        void quit();
    private:
        int pushURL(url_t& url);
        url_t popURL();
        int pushWebPage(url_t url,char* wp);
        webpage* popWebPage();
        webpage* popWebPageB();
        void P(int i){ sem_wait(&pv[i]);}
        void V(int i){ sem_post(&pv[i]);}
        void logGet(char * url);

        url_t urlPool[POOLSIZE];
        webpage webPagePool[POOLSIZE];
        sem_t pv[4];
        int urlReader,urlWriter;
        int webPReader,webPwriter;
        int fullSize;
        int threadNum;
        int state;
        FILE *logfp;
};
#endif
