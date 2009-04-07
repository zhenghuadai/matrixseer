#ifndef INFRALIBH
#define  INFRALIBH
#include "smtpsocket.h"
typedef struct {
    char * head;
    char * content;
    int length;
}http_t;
int httpget(char * url,http_t *m_http,int *errcode=NULL);
int httpget(char * url,http_t *m_http,int start,int end,int *errcode=NULL);
char *httpsock(char * url,int *errcode=NULL);
int httpDownLoad(char * url,char* fn,int start);
int httpDownLoad(char * url,char* fn);
#endif
