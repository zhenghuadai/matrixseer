/*
 * =====================================================================================
 *
 *       Filename:  testSendFile.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2008年05月09日 10时40分03秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zhenghua Dai (Zhenghua Dai), djx.zhenghua@gmail.com
 *        Company:  dzh
 *
 * =====================================================================================
 */

#include <stdlib.h>
#include "netlib.h"
char * readFile(char* fn,int * size);
int main(int argc,char *argv[])
{
char *email="djx.zhenghua@gmail.com";
char * data;
char *subj="good";
int size;
data = readFile(argv[1],&size);
Email myemail;
myemail.mailto(email,data,subj);
}

