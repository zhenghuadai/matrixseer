#include <stdio.h>
#include "stdlib.h"
#include "tagattr.h"
#include "stack.h"
#include "html.h"
#include "htmltree.h"
#include "string.h"

#define getTag \
    i=0;\
while((*pcur)&&(*pcur ==' ')) pcur ++;\
while((*pcur) &&(*pcur != ' ') &&(*pcur !='=')) {\
    buf[i++] = *pcur; pcur++;\
}\
buf[i] = 0;\
tag = -1;\
for(i =0;i<allattrNum;i++)\
{\
    int ret =strcasecmp(buf,allattrs[i]);\
    if(ret ==0){\
        tag = i;\
        break;\
    }\
}

#define getValue \
    while((*pcur)&&(*pcur !='=')) pcur ++;\
pcur++ ;\
i = 0;\
while((*pcur)&&(*pcur ==' ')) pcur ++;\
if(*pcur == '"'){\
    pcur++ ;\
    while((*pcur) &&(*pcur != '"') ) {\
        buf[i++] = *pcur; pcur++;\
    }\
    if(*pcur) pcur ++ ;\
}\
else{\
    while((*pcur) &&(*pcur != ' ') ) {\
        buf[i++] = *pcur; pcur++;\
    }\
}\
buf[i]=0;


int htmlparser::ImgAttr(char *attrstr,void ** pobj)
{
    imgattr_t* p_imgattr=(imgattr_t*)malloc(sizeof(imgattr_t));
    memset(p_imgattr,0,sizeof(imgattr_t));
    *pobj = (void *) p_imgattr;
    const static char * allattrs[]  = {"src","width","height","alt" };
    const int allattrNum = 4;
    int tag;
    char * pcur = attrstr;
    char buf[1024];
    int i;
    if(attrstr ==NULL) return 1;
    while(*pcur)
    {
        getTag;
        switch(tag){
            case 0:
                getValue;
                p_imgattr -> src = strdup(buf);
                break;
            case 1:
                getValue;
                p_imgattr -> w = atoi(buf);
                break;
            case 2:
                getValue;
                p_imgattr -> h = atoi(buf);
                break;
            case 3:
                getValue;
                p_imgattr -> alt = strdup(buf);
                break;
            default:
                getValue;
                break;
        }
    }

    return 0;
}

int htmlparser::tableAttrFunc(char *attrstr,void ** pobj)
{
    tableattr_t* p_tableattr=(tableattr_t*)malloc(sizeof(tableattr_t));
    memset(p_tableattr,0,sizeof(tableattr_t));
    *pobj = (void *) p_tableattr;
    const static char * allattrs[]  = {"cellpadding","width","height","cellspacing","bgcolor","align" };
    const int allattrNum = 6;
    int tag;
    char * pcur = attrstr;
    char buf[1024];
    int i;
    if(attrstr ==NULL) return 1;
    while(*pcur)
    {
        getTag;
        switch(tag){
            case 0:
                getValue;
                //   p_tableattr ->  = strdup(buf);
                break;
            case 1:
                getValue;
                if(buf[3] == '%') 
                    p_tableattr -> w = - atoi(buf);
                else
                    p_tableattr -> w = atoi(buf);
                break;
            case 2:
                getValue;
                p_tableattr -> h = atoi(buf);
                break;
            case 3:
                getValue;
                // p_tableattr -> alt = strdup(buf);
                break;
            case 4:
                getValue;
                p_tableattr -> bgcolor = 10;
                break;
            case 5:
                getValue;
                if(buf[0] == 'r') 
                    p_tableattr -> align = ALIGNRIGHT;
                else if(buf[0] == 'l') 
                    p_tableattr -> align = ALIGNLEFT;
                else if(buf[0] == 'c') 
                    p_tableattr -> align = ALIGNCENTER;
                printf("table align:%s\n",buf); 
                break;
            default: getValue;
                     break;
        }
    }

    return 0;
}

int htmlparser::tdAttrFunc(char *attrstr,void ** pobj)
{
    tdattr_t* p_tdattr=(tdattr_t*)malloc(sizeof(tdattr_t));
    memset(p_tdattr,0,sizeof(tdattr_t));
    *pobj = (void *) p_tdattr;
    const static char * allattrs[]  = {"align","cellpadding","width","height","cellspacing" };
    const int allattrNum = 4;
    int tag;
    char * pcur = attrstr;
    char buf[1024];
    int i;
    if(attrstr ==NULL) return 1;
    while(*pcur)
    {
        getTag;
        switch(tag){
            case 0:
                getValue;
                if(buf[0] == 'r') p_tdattr -> align = ALIGNRIGHT;
                printf("$$%s$$",buf);
                break;
            case 1:
                getValue;
                //   p_tdattr ->  = strdup(buf);
                break;
            case 2:
                getValue;
                if(buf[3] == '%') 
                    p_tdattr -> w = - atoi(buf);
                else
                    p_tdattr -> w = atoi(buf);
                break;
            case 3:
                getValue;
                p_tdattr -> h = atoi(buf);
                break;
            case 4:
                getValue;
                // p_tableattr -> alt = strdup(buf);
                break;
            default: getValue;
                     break;
        }
    }

    return 0;
}


int htmlparser::GeneralAttr(char *attrstr,void ** pobj)
{
    objBase* p_objBase=(objBase*)malloc(sizeof(objBase));
    memset(p_objBase,0,sizeof(objBase));
    *pobj = (void *) p_objBase;
    const static char * allattrs[]  = {"align","width","height","bkcolor" };
    const int allattrNum = 4;
    int tag;
    char * pcur = attrstr;
    char buf[1024];
    int i;
    printf("in attr\n");
    if(attrstr ==NULL) return 1;
    while(*pcur)
    {
        getTag;
        printf("{%s}\n",pcur);
        printf("tag[%s=",buf);
        switch(tag){
            case 0:
                getValue;
                break;
            case 1:
                getValue;
                p_objBase -> w = atoi(buf);
                break;
            case 2:
                getValue;
                p_objBase -> h = atoi(buf);
                break;
            case 3:
                getValue;
                break;
            default:
                getValue;
                break;
        }
        printf("=%s\n",buf);
    }

    return 0;
}


int htmlparser::InputAttr(char *attrstr,void ** pobj)
{
    inputattr_t * p_inputAttr=(inputattr_t*)malloc(sizeof(inputattr_t));
    memset(p_inputAttr,0,sizeof(inputattr_t));
    *pobj = (void *) p_inputAttr;
    const static char * allattrs[]  = {"name","type","value","maxlenth","size","title" };
    const int allattrNum = 6;
    int tag;
    char * pcur = attrstr;
    char buf[1024];
    int i;
    if(attrstr ==NULL) return 1;
    p_inputAttr -> type = INPUTINPUT;
    while(*pcur)
    {
        getTag;
        switch(tag){
            case 0:
                getValue;
                p_inputAttr -> name = strdup(buf);
                break;
            case 1:
                getValue;
                if(strcmp(buf,"submit")==0)
                    p_inputAttr -> type = INPUTBUTTON;
                else if(strcasecmp(buf,"radio")==0)
                    p_inputAttr -> type = INPUTRAIDO;
                else if(strcasecmp(buf,"text")==0)
                    p_inputAttr -> type = INPUTINPUT;
                else 
                    p_inputAttr -> type = INPUTUNKNOW;
                break;
            case 2:
                getValue;
                p_inputAttr -> value = strdup(buf);
                break;
            case 3:
                getValue;
                p_inputAttr -> maxlength = atoi(buf);
                break;
            case 4:
                getValue;
                p_inputAttr -> size = atoi(buf);
                break;
            case 5:
                getValue;
                p_inputAttr -> title = strdup(buf);
                break;
            default:
                getValue;
                break;
        }
    }

    return 0;
}



int htmlparser::aAttr(char *attrstr,void ** pobj)
{
    aattr_t * p_aAttr=(aattr_t*)malloc(sizeof(aattr_t));
    memset(p_aAttr,0,sizeof(aattr_t));
    *pobj = (void *) p_aAttr;
    const static char * allattrs[]  = {"href"};
    const int allattrNum = 1;
    int tag;
    char * pcur = attrstr;
    char buf[1024];
    int i;
    if(attrstr ==NULL) return 1;
    while(*pcur)
    {
        getTag;
        switch(tag){
            case 0:
                getValue;
                p_aAttr -> href = strdup(buf);
                break;
            default:
                getValue;
                break;
        }
    }

    return 0;
}



int htmlparser::pAttr(char *attrstr,void ** pobj)
{
    pTAGattr_t * p_pAttr=(pTAGattr_t*)malloc(sizeof(pTAGattr_t));
    memset(p_pAttr,0,sizeof(pTAGattr_t));
    *pobj = (void *) p_pAttr;
    const static char * allattrs[]  = {"color","id","class"};
    const int allattrNum = sizeof(allattrs)/sizeof(char*);
    int tag;
    char * pcur = attrstr;
    char buf[1024];
    int i;
    if(attrstr ==NULL) return 1;
    while(*pcur)
    {
        getTag;
        switch(tag){
            case 0:
                getValue;
                break;
            case 1:
                getValue;
                p_pAttr -> p_id = strdup(buf);
                break;
            case 2:
                getValue;
                p_pAttr -> p_class = strdup(buf);
                break;
            default:
                getValue;
                break;
        }
    }

    return 0;
}



int htmlparser::DivAttr(char *attrstr,void ** pobj)
{
    divattr_t * p_divAttr=(divattr_t*)malloc(sizeof(divattr_t));
    memset(p_divAttr,0,sizeof(divattr_t));
    *pobj = (void *) p_divAttr;
    const static char * allattrs[]  = {"startx","starty","rectw","recth","color"};
    const int allattrNum = 4;
    int tag;
    char * pcur = attrstr;
    char buf[1024];
    int i;
    if(attrstr ==NULL) return 1;
    while(*pcur)
    {
        getTag;
        switch(tag){
            case 0:
                getValue;
                p_divAttr -> startx = atoi(buf);
                break;
            case 1:
                getValue;
                p_divAttr -> starty = atoi(buf);
                break;
            case 2:
                getValue;
                p_divAttr -> w = atoi(buf);
                break;
            case 3:
                getValue;
                p_divAttr -> h = atoi(buf);
                break;
            case 4:
                getValue;
                p_divAttr -> bgcolor = 10;
            default:
                getValue;
                break;
        }
    }

    return 0;
}


