/*
 * =====================================================================================
 *
 *       Filename:  urlManager.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2008年06月25日 20时38分08秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zhenghua Dai (Zhenghua Dai), djx.zhenghua@gmail.com
 *        Company:  dzh
 *
 * =====================================================================================
 */

#include "urlManager.h"
void urlManager::loadPoint()
{
    FILE* fp = fopen("checkPoint","r");
    char buf[1024];
    int k=0;
    templateNum =0; 
    while(fgets(buf,1023,fp) != NULL){
        if(buf[0] == '#') continue;
        char *p = buf;
        while((*p) &&( *p !=' ')) p++;
        if(*p == 0) continue;
        *p= 0;
        p++;
        urlTemplatePool[k] = strdup(buf);
        urlPoolInt[k] = atoi(p);
        k++;
        templateNum ++;
    }
    fclose(fp);
}

void urlManager::prints()
{
   for(int i=0;i<templateNum;i++)
                   fprintf(stdout,"%s %d\n",urlTemplatePool[i],urlPoolInt[i]);
}
#ifdef TESTURLMAIN
int main()
{
   urlManager um;
   um.prints();
}
#endif
