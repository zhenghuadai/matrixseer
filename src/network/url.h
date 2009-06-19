/*
 * =====================================================================================
 *
 *       Filename:  url.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2008年06月23日 21时41分43秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zhenghua Dai (Zhenghua Dai), djx.zhenghua@gmail.com
 *        Company:  dzh
 *
 * =====================================================================================
 */
#ifndef __URL_HEADER__
#define __URL_HEADER__
#include "debprintf.h"
class url_t
{
    public:
    url_t(){}
    url_t(char*p,int t){url =p;type=t;}
    int hash();
    public:
    char *url;
    int type;
};
#endif
