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

#define POOLSIZE 1024
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include "netlib.h"

typedef struct _webpage
{
    char * url;
    char * content;
    int length;
    ~_webpage(){delete content;delete url;};
}webpage;

class downLoader 
{
    public: 
        downLoader();
        ~downLoader();
        int push(char *url){return pushURL(url);};
        webpage* pop(){return popWebPage();};
        void run();

        pthread_t runThread;
        sem_t semURLPoolEmpty;
    private:
        int pushURL(char *url);
        char* popURL();
        int pushWebPage(char *url,char* wp);
        webpage* popWebPage();
        int isURLPoolFull(){int tmpW = (urlWriter+1)%fullSize; return (tmpW == urlReader);};
        int isURLPoolEmpty(){return (urlReader == urlWriter);};
        int isWPPoolFull(){int tmpW = (webPwriter+1)%fullSize; return (tmpW == webPReader);};
        int isWPPoolEmpty(){return (webPReader == webPwriter);};

        char* urlPool[POOLSIZE];
        webpage webPagePool[POOLSIZE];
        int urlReader,urlWriter;
        int webPReader,webPwriter;
        int fullSize;
};

