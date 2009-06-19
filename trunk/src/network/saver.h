/*
 * =====================================================================================
 *
 *       Filename:  saver.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2008年06月27日 23时38分55秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zhenghua Dai (Zhenghua Dai), djx.zhenghua@gmail.com
 *        Company:  dzh
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
void getdate(char *buf){
    //char *wday[]={"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
    time_t timep;
    struct tm *p;
    time(&timep);
    p=gmtime(&timep);
    sprintf(buf,"data-%d-%d-%d",(1900+p->tm_year), (1+p->tm_mon),p->tm_mday);
    //printf("data-%d-%d-%d",(1900+p->tm_year), (1+p->tm_mon),p->tm_mday);
    //fflush(stdout);
    //printf("%s%d;%d;%d\n", wday[p->tm_wday], p->tm_hour, p->tm_min, p->tm_sec);
}

class saver
{
    public:
        saver() {
            char *fn = new char[64];
            getdate(fn);
            savefp = fopen(fn,"a");if(savefp ==NULL) {printf("open err\n");}
            delete fn;
        }

        saver(char *fn) {savefp = fopen(fn,"w");if(savefp ==NULL) {printf("open err\n");}}
        ~saver(){fclose(savefp);}
        void    print(char *fmt,...){
            va_list args;
            va_start(args,fmt);
            vfprintf(savefp,fmt,args);
            va_end(args);
            fflush(savefp);
        }
    private:
        FILE* savefp;
};
