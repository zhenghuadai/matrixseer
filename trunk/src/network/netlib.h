#ifndef INFRALIBH
#define  INFRALIBH
#include "smtpsocket.h"
typedef struct {
    char * head;
    char * content;
    int length;
}http_t;
int httpget(char * url,http_t *m_http,int *errcode=0);
char *httpsock(char * url,int *errcode=0);
char *httpget(char * url,int *errcode=0);
char *smthGetContentFromsTx(char *sTx,int lenSource=0);
#endif
