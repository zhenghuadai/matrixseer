/*
 * =====================================================================================
 *
 *       Filename:  testTimer.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2008年05月13日 21时57分05秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zhenghua Dai (Zhenghua Dai), djx.zhenghua@gmail.com
 *        Company:  dzh
 *
 * =====================================================================================
 */
#include <time.h>
#include <sys/time.h>
#include <sys/signal.h>
#include <stdlib.h>
#include <stdio.h>
#define rdtsc(low,high) __asm__ __volatile__("rdtsc" : "=a" (low), "=d" (high))

timer_t tt;

void timeHandler (int signo)
{
    static int last_i=0;
    unsigned int i, j;
    //if(signo != SIGALRM) return ;
    last_i++;
    fprintf(stdout,"%d",last_i);
    fflush(stdout);
    if(last_i==10) setitimer(ITIMER_REAL,NULL,NULL);
    signal(SIGALRM,timeHandler);
}

int setTimer ()
{
    struct itimerval value2,ovalue;

    signal(SIGALRM,timeHandler);
    value2.it_value.tv_sec = 0;
    value2.it_value.tv_usec = 500;
    value2.it_interval.tv_sec = 0;
    value2.it_interval.tv_usec = 500000;
    setitimer(ITIMER_REAL, &value2, &ovalue);
}

int main()
{
 setTimer ();
    while(1){};
    return 1;
}
