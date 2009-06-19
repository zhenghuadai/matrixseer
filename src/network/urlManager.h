/*
 * =====================================================================================
 *
 *       Filename:  url.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2008年06月23日 21时14分51秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zhenghua Dai (Zhenghua Dai), djx.zhenghua@gmail.com
 *        Company:  dzh
 *
 * =====================================================================================
 */
#ifndef __URLMANAGER__HEADER__
#define __URLMANAGER__HEADER__
#include <stdio.h>
#include <stdlib.h>
#include "url.h"
#include <string.h>
#include "downLoader.h"
#define TEMPLATEPOOLSIZE 100
#define smthTicketTemplate "http://www.newsmth.net/bbscon.php?bid=833&id=%d"
//78554
#define smthCareerTemplate "http://www.newsmth.net/bbscon.php?bid=943&id=%d"
//66588
class urlManager
{
    public:
        urlManager(){ 
            loadPoint();
            for(int i=0;i<TEMPLATEPOOLSIZE;i++){
                urlPool[i].url = urlstrPool[i];
                urlPool[i].type = i;
                errNums[i] =0;
            }
        }
        void initTemplate()
        { 
            urlTemplatePool[0] = smthTicketTemplate; urlPoolInt[0] = 78554;
            urlTemplatePool[1] = smthCareerTemplate; urlPoolInt[1] = 66588;
            templateNum =2;
        }
        void savePoint(){
            FILE* fp = fopen("checkPoint","w");
            for(int i=0;i<templateNum;i++)
                fprintf(fp,"%s %d\n",urlTemplatePool[i],urlPoolInt[i]);
            fclose(fp);
        };
        void loadPoint();
        void createURL(){
            for(int i=0;i<templateNum;i++) 
                sprintf(urlstrPool[i],urlTemplatePool[i],urlPoolInt[i]);
        };
        void forwardURL(int id){errNums[id] =0;urlPoolInt[id] ++;};
        void errforwardURL(int id){
            errNums[id] ++;
            if(errNums[id] < 10 )
            {
                urlPoolInt[id] ++;
            }else{
                urlPoolInt[id] -=(10 -1);
            };
        }
        void sendURL(downLoader* downer){ 
            for(int i=0;i<templateNum;i++){ 
                downer -> push(urlPool[i]);
                debprintf("send %s\n",urlPool[i].url);
            }
        }
        void prints();
        void printsurl();
        char  urlstrPool[TEMPLATEPOOLSIZE][1024];
        url_t urlPool[TEMPLATEPOOLSIZE];
        int urlNum(){return templateNum;}
    private:
        int   urlPoolInt[TEMPLATEPOOLSIZE];
        int   errNums[TEMPLATEPOOLSIZE];
        char *urlTemplatePool[TEMPLATEPOOLSIZE];
        int templateNum;

};
#endif
