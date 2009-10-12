/*
 * =====================================================================================
 *Copyright(C) by Zhenghua Dai. All rights reserved.
 *
 *
 *
 * =====================================================================================
 */
/*
 * =====================================================================================
 *
 *       Filename:  taghash.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2009年10月11日 17时29分59秒
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
#include "../htmltag.h"
#include "string.h"
#include "str2hash.h" 
#define HASHLEN 256 
#define HASHLENMASK HASHLEN 

static char *alltag[]=INITTAG;
static char tagHash[HASHLEN] ={ 0};

unsigned char hash(char* tag)
{
    int v = 0;
    int len = strlen(tag);
    for(int i=0;i< len; i++){
        v += (tag[i]);
    }
    v += len;
    //v = v% (HASHLEN/4);
//
    //switch( len) {
    //    case 1:
    //    case 2:
    //        break;
    //    case 3:
    //    case 4:
    //        v = (v) | (1<<8);
    //        break;
    //    case 5:
    //    case 6:
    //        v = (v ) | (2<<8);
    //        break;
    //    default:
    //        v = (v ) | (3<<8);
    //        break;
    //}
    return v % HASHLENMASK;
}

int main(int argc, char** argv)
{
    for(int i=0 ; i< MAXTAGID;i++){
        unsigned char v = hash(alltag[i]);
        tagHash[v] ++;
        printf("[%d %s: %d]\n", i, alltag[i], v);
    }
    for(int i=0;i<HASHLEN;i++){
        if(tagHash[i]) printf("<%d %d>\n", i, tagHash[i]);
    }
	Str2hash str2hash(alltag, MAXTAGID);
	str2hash.print();
	#define test(s) id=str2hash.get(s);printf("%d %s\n", id, s);
	int id = str2hash.get("a");
	printf("%d %s\n", id, "a");
	test("/a");
	test("/abc");
}
