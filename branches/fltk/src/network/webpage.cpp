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
	fullSize = POOLSIZE;
}

downLoader::~downLoader()
{
	//pthread_join(runThread,NULL);
}
int downLoader::pushURL(char *url)
{
	if(isURLPoolFull()) return -1;
	if(isURLPoolEmpty()) sem_post(&semURLPoolEmpty);
	urlPool[urlWriter] = strdup(url);
	urlWriter =(urlWriter+1)%fullSize;
	return 0;
}

char* downLoader::popURL()
{
	if(isURLPoolEmpty())
		return (char*)  NULL;
	int tmpReader = urlReader;
	urlReader = (urlReader + 1) % fullSize;
	return 	urlPool[tmpReader];
}

int downLoader::pushWebPage(char *url,char *wp)
{
	if(isWPPoolFull()) return -1;
	int tmpWriter = webPwriter;
	webPagePool[tmpWriter].url = url;
	webPagePool[tmpWriter].content = wp;
	webPwriter = (webPwriter + 1) % fullSize;
	return 0;
}

webpage* downLoader::popWebPage()
{
	if(isWPPoolEmpty()) return (webpage*)NULL;
	int tmpReader = webPReader;
	webpage * tmpwp = new webpage;
	tmpwp->url = webPagePool[tmpReader].url;
	tmpwp->content = webPagePool[tmpReader].content;
	webPReader = (webPReader + 1) % fullSize;
	return tmpwp;
}

void downLoader::run()
{
	char*	tmpurl;
	//while((tmpurl= popURL())!=NULL)
	while(1){
		tmpurl = popURL();
		while(tmpurl == NULL) {
			int waitTime = 0;
			while(waitTime <10){
				int x;
				waitTime ++;
				tmpurl = popURL();
				if (tmpurl !=NULL) 
					goto GetURL;
				else
					noop(&x);
			}
			sem_wait(&semURLPoolEmpty);
			tmpurl = popURL();
		}
GetURL:
		if(tmpurl != NULL)	{
            pushWebPage(tmpurl,httpget(tmpurl));
        }
	}
}

downLoader mDL;
void* runner(void* threadID)
{
	mDL.run();
	return (void*) 0;
}

void runDownLoader()
{
	pthread_create(&(mDL.runThread) ,NULL,runner,NULL); 	
	sem_init(&(mDL.semURLPoolEmpty),0,0);
}
#ifdef  downLoaderTest
int main(int argc,char **argv)
{
	char*	url = "www.google.com";
	char*	url1 = "http://www.newsmth.net/bbscon.php?bid=978&id=366684";
	mDL.push(url);
	//mDL.push(url1);
	//mDL.run();
	runDownLoader();
	//sleep(2);
	webpage* wp;
	//mDL.push(url1);
	//mDL.push(url1);
	mDL.push(url1);
	while (1){
		wp= mDL.pop();
		if(wp !=NULL)printf("%s\n%s\n",wp->url,smthGetContentFromsTx(wp->content));
        delete wp;
	}
	pthread_join(mDL.runThread,NULL);
}
#endif
