#include<stdio.h>
#include<stdlib.h>
#include<string.h>
//#include "htmltag.h"
#include "stack.h"
#include "tagattr.h"
#include "html.h"
#include "math.h"
static char *alltag[]=INITTAG;
static char *allattr[]=INITATTR;
#define CALLFUNC(tag)
//{"html","/html","head","/head", "title","/title" ,"body","/body" ,"","br","script","/script"};
void Ffree(char **pp)
{
    if(*pp){
        free(*pp);
        *pp=0;
    }
}
htmlparser::htmlparser()
{
    initialFunc();
    phead = 0;
    tagbufp = (char *)malloc(1024);
    debprintf("class func size:%d",sizeof(tagfunc_t));
}
htmlparser::~htmlparser()
{
    Ffree(&tagbufp);
}
int htmlparser::releaseNode(pHtmlNode root)
{	
    if(attrOfHtmlNode(root) )free(attrOfHtmlNode(root));
    if(root -> value) free(root -> value);
    free(root);
}
int htmlparser::releaseTree(pHtmlNode root)
{
    if( root -> child) releaseTree(root -> child);
    if( root -> sib)   releaseTree(root -> sib);
    releaseNode(root);
}
int htmlparser::destroy()
{
    releaseTree(phead);	
}

char *htmlparser::gettagstrfromid(int id)
{
    return alltag[id];
}

htmlparser::ctTagFunc_t htmlparser::ctTagFunc[100] ={0};

void htmlparser::initialFunc()
{
    int i;
    static int isFirst = 1;
    for(i=0;i<100;i+=2)
    {
        tagfunc[i]=&htmlparser::Sgeneralfunc;
        tagfunc[i+1]=&htmlparser::Egeneralfunc;
        if(isFirst){
            ctTagFunc[i]=htmlparser::ctSfunc;
            ctTagFunc[i+1]=htmlparser::ctEfunc;
        }
    }
    if(isFirst){
        //create tree
        _ctAF2(HTMLID,ctShtmlfunc,ctEhtmlfunc);
        _ctAF1(IMGID,ctUnaryfunc);
        _ctAF1(METAID,ctUnaryfunc);
        _ctAF1(LINKID,ctUnaryfunc);
        _ctAF1(SCRIPTID,ctSscriptfunc);
        _ctAF1(SCRIPTID+1,ctEscriptfunc);
        _ctAF1(TABLEID,ctStablefunc);
        _ctAF1(TABLEID+1,ctEtablefunc);
        _ctAF1(TRID,ctStrfunc);
        _ctAF1(TRID+1,ctEtrfunc);
        _ctAF1(TDID,ctStdfunc);
        _ctAF1(TDID+1,ctEtdfunc);
        _ctAF2(ULID,ctSulfunc,ctEulfunc);
        _ctAF1(IMGID,ctImgfunc);
        _ctAF1(BRID,ctBrfunc);
        _ctAF1(INPUTID,ctInputfunc);
        _ctAF2(ATAGID,ctSafunc,ctEafunc);
        _ctAF2(PTAGID,ctSpfunc,ctEpfunc);
        _ctAF2(DIVID,ctSdivfunc,ctEdivfunc);
        _ctAF2(SPANID,ctSspanfunc,ctEspanfunc);
        _ctAF2(FORMID,ctSformfunc,ctEformfunc);
        _ctAF2(FONTID,ctSfontfunc,ctEfontfunc);
        _ctAF2(BTAGID,ctSbfunc,ctEbfunc);
        _ctAF2(UTAGID,ctSufunc,ctEufunc);
        _ctAF2(CENTERID,ctScenterfunc,ctEcenterfunc);
        _ctAF2(NOBRID,ctSnobrfunc,ctEnobrfunc);
        _ctAF2(SMALLID,ctSsmallfunc,ctEsmallfunc);
        _ctAF2(STYLEID,ctSstylefunc,ctEstylefunc);
        _ctAF2(HEADID,ctSheadfunc,ctEheadfunc);
        _ctAF2(TITLEID,ctStitlefunc,ctEtitlefunc);
        _ctAF2(H2ID,ctSskipfunc,ctEskipfunc);
    }
    isFirst = 0;
    //render tree
    tagfunc[HTMLID]=&htmlparser::Shtmlfunc;
    tagfunc[HTMLID+1]=&htmlparser::Ehtmlfunc;
    _AF2(SCRIPTID,Sscriptfunc,Escriptfunc);
    _AF2(HEADID,Sheadfunc,Eheadfunc);
    _AF1(BRID,brfunc);
    _AF2(PTAGID,Spfunc,Epfunc);
    _AF2(STYLEID,Sstylefunc,Estylefunc);
    _AF1(IMGID,imgfunc);
    _AF1(METAID,metafunc);
    _AF2(TABLEID,Stablefunc,Etablefunc);
    _AF2(TRID,Strfunc,Etrfunc);
    _AF2(TDID,Stdfunc,Etdfunc);
    _AF2(ULID,Sulfunc,Eulfunc);
    _AF2(LIID,Slifunc,Elifunc);
    _AF2(ATAGID,Safunc,Eafunc);
    _AF1(VALUEOBJID,Evaluefunc);
    _AF1(INPUTID,inputfunc);
    _AF2(DIVID,Sdivfunc,Edivfunc);
}
int htmlparser::gettag(char *tag)
{
    int i;
    i=0;
    if(tag ==NULL) return -1;
    if(*tag ==0) return -1;
    while(strcasecmp(tag,alltag[i])!=0)
    {
        i++;
        if(i>MAXTAGID)break;
    }
    if(i>MAXTAGID)
    {
        return -1;
    }

    return i;
}

void htmlparser::_printstack()
{
    int i;
    for(i=0;i<=_tagtop;i++)
        debprintf("%d ",_3dhstack[i] -> tagid);
    for(i=0;i<=_tagtop;i++)
        debprintf(" %s ",gettagstrfromid(_3dhstack[i] -> tagid));
}

char * htmlparser::gettagstr(char **pos)
    /*
     * pos is on '<'
     *
     */
{
    char *pre=(*pos) +1;
    //	char *tagp;
    //if(**pos != '<') return (char *)-1;
    while(*pre ==' ') pre++;
    char *pcur=pre;
    while((*pcur)&&(*pcur!=' ')&&(*pcur!='>')&&(*pcur!='<')&&(*pcur!='\n'))
    {
        pcur++;
    }

    //    if(! *pcur)break;
    int slen=pcur-pre;
    //              tagp=(char *)malloc(slen+1);
    memcpy(tagbufp,pre,slen);
    tagbufp[slen]=0;
    *pos=pcur;
    return tagbufp;



}
char * htmlparser::getattrstr(char **pos)
{
    char *pre=(*pos) ;
    char *pcur=pre;
    char *attributes;
    while((*pcur)&&(*pcur!='>')&&(*pcur!='<'))
    {
        pcur++;
    }
    // if(! *pcur)break;
    int slen=pcur-pre;
    if(!slen)return 0;
    attributes=(char *)malloc((slen));
    memcpy(attributes,pre+1,slen-1);
    attributes[slen-1]=0;
    *pos=pcur;
    return attributes;
}
char * htmlparser::getvaluestr(char **pos,int *nexttag,int gototag)
{
    /*
     *pos is on '>'
     *
     *
     *
     */
    char *pre=(*pos) +1;
    char *pcur=pre;
    char *tagp;
    char *pend;
valuegoon:
    while((*pcur)&&(*pcur!='<'))
    {
        pcur++;
    }
    char *tmpcur=pcur;
    tagp=gettagstr(&pcur);
    int tagid;
    tagid=gettag(tagp);
    if((tagid != gototag)&&(*pcur)) goto valuegoon;
    pend=tmpcur;
    int    slen=pend-pre;
    char *value=0;
    if(!slen)
    {
        *nexttag=tagid;
        *pos=pcur;
        return 0;
    }
    {
        value=(char *)malloc((slen+1));
        int i,j=0;
#if 1
        for( i=0;i<slen;i++)
        {
            if((pre[i]=='&')&&(pre[i+1]=='n')&&(pre[i+2]=='b')&&(pre[i+3]=='s')&&(pre[i+4]=='p'))
            {
                value[j++]=' ';
                i+=4;
                if(pre[i+1]==';')i++;
            }
            else if(pre[i]!='\n')
                value[j++]=pre[i];
        }
#else
        memcpy(value,pre,slen);
#endif
        value[j]=0;
        if(j==0)
        {
            free(value);
            *pos=pcur;
            *nexttag=tagid;
            return 0;
        }
    }
    *pos=pcur;
    *nexttag=tagid;
    return value;

}

char * htmlparser::getvaluestr(char **pos,int *nexttag)
{
    char *pre=(*pos) +1;
    char *pcur=pre;
    char *tagp;
    char *pend;
valuegoon:
    while((*pcur)&&(*pcur!='<'))
    {
        pcur++;
    }
    char *tmpcur=pcur;
    tagp=gettagstr(&pcur);
    int tagid;
    tagid=gettag(tagp);
    if((tagid==-1)&&(*pcur)) goto valuegoon;
    pend=tmpcur;
    int    slen=pend-pre;
    char *value=0;
    if(!slen)
    {
        *nexttag=tagid;
        *pos=pcur;
        return 0;
    }
    {
        value=(char *)malloc((slen+1));
        int i,j=0;
#if 1
        for( i=0;i<slen;i++)
        {

            if((pre[i]!='&')&&(pre[i]!='\n'))
                value[j++]=pre[i];
            else if(pre[i]=='&'){
                if((pre[i+1]=='n')&&(pre[i+2]=='b')&&(pre[i+3]=='s')&&(pre[i+4]=='p'))
                {
                    value[j++]=' ';
                    i+=4;
                    if(pre[i+1]==';')i++;
                }
                else if((pre[i+1]=='c')&&(pre[i+2]=='o')&&(pre[i+3]=='p')&&(pre[i+4]=='y')&&(pre[i+5]==';')){
                    value[j++]=' ';
                    i+=5;
                }
            }
        }//end for
#else
        memcpy(value,pre,slen);
#endif
        value[j]=0;
        if(j==0)
        {
            free(value);
            *pos=pcur;
            *nexttag=tagid;
            return 0;
        }
    }
    *pos=pcur;
    *nexttag=tagid;
    return value;

}

char * htmlparser::getvaluestrp(char **pos,int *nexttag)
{
    char *pre=(*pos) +1;
    char *pcur=pre;
    char *tagp;
    char *pend;
valuegoon:
    while((*pcur)&&(*pcur!='<'))
    {
        pcur++;
    }
    char *tmpcur=pcur;
    tagp=gettagstr(&pcur);
    int tagid;
    tagid=gettag(tagp);
    if((tagid==-1)&&(*pcur)) goto valuegoon;
    pend=tmpcur;
    int    slen=pend-pre;
    char *value=0;
    if(!slen)
    {
        *nexttag=tagid;
        *pos=pcur;
        return 0;
    }
    {
        value=(char *)malloc((slen+1));
        int i,j=0;
#if 0 
        for( i=0;i<slen;i++)
        {/*
            if((pre[i]=='&')&&(pre[i+1]=='n')&&(pre[i+2]=='b')&&(pre[i+3]=='s')&&(pre[i+4]=='p'))
            {
            value[j++]=' ';
            i+=4;
            }
            else if(pre[i]!='\n')
            */
            value[j++]=pre[i];
        }
#else
        memcpy(value,pre,slen);
        j=slen;
#endif
        value[j]=0;
        if(j==0)
        {
            free(value);
            *pos=pcur;
            *nexttag=tagid;
            return 0;
        }
    }
    *pos=pcur;
    *nexttag=tagid;
    return value;

}


#define jmptoc(pcur,c) \
{                        \
    while((*pcur)&&(*pcur!=c)) \
    {                          \
        pcur++;            \
    }                          \
}                                   
inline int isendtag(int tag)
{
    return (tag & 1);
}
int htmlparser::parsehtml(char *htdoc)
{
    char * pcur=htdoc;
    char *pre;
    char buf[1024];
    int inbody=1,curtag=-1;
    int i=0;
    static int nums=0;
#ifdef DEB
    printf("start parse.................................\n");
    //printf("%s\n",htdoc);
    printf(".............................................\n");
    fflush(stdout);
#endif
    while(*pcur)
    {
        //while((*pcur==' ')||(*pcur=='\t')||(*pcur=='\n'))pcur++;
        char *tagp=0;
        char *attributes=0;
        char *value=0;
        int slen,ret;
        pre=pcur;
searchtag:
        jmptoc(pcur,'<');
        if(! *pcur)break;

        tagp=gettagstr(&pcur);
        curtag=gettag(tagp);
        //printf("tag:%d\n",curtag);
        if(curtag==-1) 
        {
            goto searchtag;
        }
findouttag:
        // find out a tag
#ifdef DEB
        debprintf("\n\n\033[35mTAG\033[0m:<%s>",gettagstrfromid(curtag));
#endif
        attributes = NULL;
        if(*pcur!='>')	
        {
            attributes=getattrstr(&pcur);
        }
#ifdef DEB
            printf("\033[35mATTRIB\033[0m:[%s]",attributes);
#endif
        ret=isendtag(curtag);
        int funcret;
        if(ret==ENDTAG)
        {
            // process endTag
            dmToken t(curtag, attributes,NULL);
            //funcret = processEndTag(curtag,attributes,ATTRIB,NULL);
            funcret = processEndTag(&t, ATTRIB);
            //printf("tag not match:%s\n",gettagstrfromid(curtag));
#ifdef DEB
            printf("\n\033[33mstack:");
            _printstack();
            printf("\033[0m\n");
            fflush(stdout);
            printf("\n");
#endif
            curtag=gettagfromstack();
            if(curtag == -1) return 0;
        }
        int nexttag;
scanValue:
        nexttag = -1;
        value = 0;
        if((curtag !=PTAGID)&&(curtag !=SCRIPTID)){
            value=getvaluestr(&pcur,&nexttag);
        }
        else if(curtag ==PTAGID)
            value=getvaluestrp(&pcur,&nexttag);
        else if(curtag ==SCRIPTID){
            value=getvaluestr(&pcur,&nexttag,curtag+1);
        }
#ifdef DEB
        if(value)
        {
            if(ret==ENDTAG)
                printf("\033[34mTAG\033[0m:%s;\033[35mVALUE\033[0m:%s\t",gettagstrfromid(curtag),value);
            else
                printf("\033[35mVALUE\033[0m:%s\t",value);
        }
#endif
        if((ret != ENDTAG)/*&&(nexttag == curtag +1)*/){ //! The last tag is not endTag
            if(nexttag == (curtag +1)){
                //!simple tag
                dmToken t(curtag, attributes, value);
                //processSimpleTag(curtag,attributes,TAGCOMPLETE,value);
                processSimpleTag(&t, TAGCOMPLETE);
                curtag=gettagfromstack();
                if(curtag == -1) return 0;
                ret = ENDTAG;
#ifdef DEB
            printf("\n\033[33mstack:");
            _printstack();
            printf("\033[0m\n");
            fflush(stdout);
            printf("\n");
#endif
                goto scanValue;
            }
            else{
                //debprintf("pattern:<>...<>\n");
                //processTag(curtag, attributes,ATTRIBVALUE,value);
                dmToken t(curtag, attributes, value);
                processTag(&t, ATTRIBVALUE);
            }
        }
        else if(ret == ENDTAG){
            //! the last is endTag, so this value is belong to previous; 
            //processValueOfTag(curtag, NULL,VALUE,value);
            
            dmToken t(curtag,NULL, value);
            processValueOfTag(&t,VALUE);
        }
#ifdef DEB
            printf("\n\033[33mstack:");
            _printstack();
            printf("\033[0m\n");
            fflush(stdout);
            printf("\n");
#endif
        fflush(stdout);

        if(*pcur)
        {
            curtag=nexttag;
            goto  findouttag;
        }
        //Ffree(&tagp);
        //Ffree(&attributes);
        //Ffree(&value);
        nums++;
#ifdef DEB
        fflush(stdout);
#endif

    }
}
void htmlparser::visitHtmlNode(pHtmlNode root)
{
    int tag = root -> tagid;
	Widget* o = (Widget*) root->obj;
    if( o == NULL) return ;
    if(o -> _vptr == (void*)0x1) return ;
    o -> draw();
    //(this ->* tagfunc[tag])(root );

    //    if((root -> value) ||(tag == BRID) )	(this ->* tagfunc[tag])(root );
    //  if( tag == INPUTID) (this ->* tagfunc[tag])(root);

    /*
       if((root -> value) ||(tag == BRID) )	(this ->* tagfunc[tag])(root -> value,(int )(root -> attr.value));
       if( tag == INPUTID) (this ->* tagfunc[tag])(root->attr.value,VALUE);
       */
}
void htmlparser::rendernode(pHtmlNode root)
{
    visitHtmlNode(root);
    if(root->child)	rendernode(root->child);
    if(root-> wife) { /*	(this ->* tagfunc[root->wife])(root )*/};
    if(root->sib)   rendernode(root->sib);

}
void htmlparser::renderhtml()
{
    if(phead)   rendernode(phead);	
}

int initalparser()
{
    return 1;
}

void parsehtml(char * htdoc)
{

}
