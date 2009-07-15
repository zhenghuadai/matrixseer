#include <stdio.h>
#include "html.h"
#include "stack.h"
#include "string.h"
#include "app.h"
#include "jsglobal.h"
#include "Document.h"
#ifndef __HTMLMAIN  
#include "ctrinput.h"
#include "ctrA.h"
#include "ctrP.h"
#include "ctrDiv.h"
#include "ctrText.h"
#include "display.h"
#include "debprintf.h"
#include "css.h"
#include "stackTemplate.h"
#endif
#include "htmltree.h"
extern int rowHeight;
extern int charWidth;
#define SKIPTHISTAG\
    int curtag =curHtml-> gettagfromstack();\
if(which==ATTRIB){\
}\
else if(which==VALUE){\
    dmToken upT(curtag, NULL, tp->value);\
    (curHtml->ctTagFunc[curtag])(curHtml,&upT ,VALUE);\
}
    
#define  callBrfunc(curHtml) {\
    dmToken t(BRID, NULL ,NULL); \
    curHtml -> ctBrfunc(curHtml,&t,ATTRIB);\
}

void resetTablePos(pHtmlNode tableP)
{
    int i=0,j=0;
    ctrDiv *pctrTR;
    ctrDiv *pctrTD;
    ctrDiv *pctrTable;
    pHtmlNode pHtr,pHtd;
    int x,y,z,w,h;
    tableattr_t *tableAttr = (tableattr_t*)tableP -> attr;
    gridPos_t *colsW,*rowsH;
    colsW = tableAttr -> colsW;
    rowsH = tableAttr -> rowsH;
    pctrTable=(ctrDiv*) (tableP -> obj );
    pctrTable -> begin();
    pHtr = tableP -> child;
    w = tableAttr -> w;
    while(pHtr)
    {
        pctrTR = (ctrDiv*) pHtr -> obj;
        y = rowsH[i/*pHtr->rowID*/].offset; 
        pctrTR -> setxyz(0,y,0);
        //pctrTR -> sety(y);
        pctrTR -> w(w);
        pctrTR -> begin();
        pHtd = pHtr -> child;
        j =0;
        while(pHtd){
            pctrTD = (ctrDiv*) pHtd -> obj; 
            //x = colsW[pHtd -> colID].offset;
            x = colsW[j].offset;
            y = 0;
            if( ((tdattr_t*) pHtd-> attr )-> align == ALIGNRIGHT){
                int padding = colsW[j].length - pctrTD->w();
                x += padding ;
            }
            pctrTD -> setxyz(x,y,0);
            //pctrTD -> setx(x);
            pHtd = pHtd -> mNext;
            j++;
        }
        pHtr = pHtr -> mNext;
        pctrTR -> end();
        i++;
    }
    pctrTable -> end();
}

    template<typename T>
void setCSS(T *pctrP,cssAttr_t *pcss)
{
    int i;
    for(i=0;i<pcss -> len; i++){
        switch(pcss->value[i].id){
            case leftID:
                pctrP->setFixX(atoi(pcss->value[i].value));
                break;
            case topID:
                pctrP->setFixY(atoi(pcss->value[i].value));
                break;
            default:
                break;
        }
    }
}

void HNinsert(pHtmlNode pHtp,pHtmlNode pHtc)
{
    if((! pHtp) || (! pHtc))return;
    pHtp->addChild(pHtc);
}

int htmlparser::processValueOfTag(dmTokenP tp,int which)
{
    int funcret;
    if(tp->value){ 
        //(ctTagFunc[tp->tagid])(this, tp, VALUE);
        char* value = tp -> value;
        pHtmlNode pHtp=_3dhtop();
        ctTextFunc(pHtp,value);
    }
    return funcret;
}
#if 1
#define  insertTextNode(curHtml,value) \
{\
        pHtmlNode pHtp=curHtml->_3dhtop();\
        curHtml->ctTextFunc(pHtp,value);\
}
#else 
#define  insertTextNode(curHtml,value) 
#endif
//static inline void insertTextNode(htmlparser* curHtml,char*  value)
//{
//        pHtmlNode pHtp=curHtml->_3dhtop();
//        curHtml->ctTextFunc(pHtp,value);
//}

int htmlparser::processTag(dmTokenP tp,int which)
{
    int funcret;
    funcret =  (ctTagFunc[tp->tagid])(this, tp, ATTRIB);
    //if(tp ->value)  (ctTagFunc[tp->tagid])(this, tp, VALUE);
    processValueOfTag(tp,VALUE);
    return funcret;
}

int htmlparser::processSimpleTag(dmTokenP tp,int which)
{
    int funcret;

    if(tp->tagid == ATAGID){
        funcret =  (ctTagFunc[tp->tagid])(this,tp,TAGCOMPLETE);
    }else{
        funcret =  (ctTagFunc[tp->tagid])(this, tp, ATTRIB);
        //if(tp->value)  (ctTagFunc[tp->tagid])(this, tp, VALUE);
        processValueOfTag(tp,VALUE);
        dmToken nextT(tp->tagid +1,NULL,NULL);
        funcret =  (ctTagFunc[(tp->tagid )+1])(this, &nextT, ATTRIB);
    }
    return funcret;
}

int htmlparser::processEndTag(dmTokenP tp,int which)
{
    int funcret;
    funcret = (ctTagFunc[tp->tagid])(this,tp, which);
    return funcret;
}

#undef OP
#undef ctOP
#define ctOP(a) int htmlparser::a(htmlparser* curHtml ,dmTokenP tp, int which) \
{\
    int tagid = tp -> tagid; \
    char* attrib= tp -> attrib;\
    char* value= tp -> value;

    //char* htdoc = tp -> attrib;\
    char* htdoc2 = tp -> value;

#define endctOP() }
int htmlparser::ctTextFunc(pHtmlNode pHtp,char * htdoc)
{
    extern int2 getstrWH(char *);
    attrBase_t *pAttrp = getAttrBaseP(pHtp); 
    pHtmlNode pHtc=createHtmlNode(VALUEOBJID);
    HNinsert(pHtp,pHtc);
    void setdefaultfont(void);
    setdefaultfont();
    pHtc -> value = htdoc;
    int2 wh = getstrWH(htdoc);
    int2 ctrwh;
    int x,y;
    GetCurXYFromPattr(x,y,pAttrp); 
    //wh.x = wh.x +5;
    ctrwh.x = wh.x + 0;
    ctrText * pctrText = new ctrText(x,y,ctrwh.x,(wh.y)*rowHeight + rowHeight/2,htdoc);
    debprintf("%d %d\n",wh.x,wh.y);
    //ctrText * pctrText = new ctrText(10,10,100,100,"the text");
    debprintf("Text size:%d %d %d %d <%s>\n",0,0,ctrwh.x,wh.y * rowHeight,htdoc);
    pHtc -> obj = (void*) pctrText;
    if(pHtp -> attr )
    {
        pAttrp -> curW += ctrwh.x;//strlen(htdoc) * charWidth;
        //if(rowHeight > pAttrp -> curH) pAttrp -> curH = rowHeight;
        if(wh.y *rowHeight> pAttrp -> curH) pAttrp -> curH = (wh.y)*rowHeight + rowHeight/2;
    }
}

ctOP(ctSfunc)
{
    if(which==ATTRIB){
        void * tmpAttrp = (void*)malloc(sizeof(attrBase_t));
        memset(tmpAttrp,0,sizeof(attrBase_t));
        pHtmlNode pHtp = curHtml->_3dhtop();
        curHtml->_3dhpush(tagid,tmpAttrp);
        pHtmlNode pHtc=curHtml-> _3dhtop();
        HNinsert(pHtp,pHtc);
    }
    else if(which==VALUE){
        insertTextNode(curHtml, value);
    }
    return 1;
}
endctOP() 

ctOP(ctEfunc)
{
    if( curHtml->_stackisSpouse(tagid)){	
        pHtmlNode pHtc = curHtml-> _3dhpop();
        //pHtmlNode pHtp = curHtml->_3dhtop();
        //HNinsert(pHtp,pHtc);
        return 1;
    }
    return 0;
}
endctOP() 

ctOP(ctSscriptfunc)
{
    char * myscript=value;//strdup(htdoc);
    if(which==ATTRIB){
    }
    else if(which==VALUE){
        //	printf("%s\n",myscript);

        jsval retval;
        JSString *str;
        uintN lineno=0;
        JSBool ok ;
        ok= JS_EvaluateScript(cx,globalObj,myscript,strlen(myscript),"abc",lineno,&retval);
        if(ok == JS_TRUE)
        {
            str = JS_ValueToString(cx,retval);
            char *s =JS_GetStringBytes(str);
            //		printf("result:%s\n",s);
        }
        else{
            //		printf("JS no result\n");
        }
    }
    return 1;
}
endctOP() 

ctOP(ctEscriptfunc)
{
    extern Document *gdocument;
    gdocument ->resetphtml();
    char * ph = gdocument -> phtml;
    curHtml->parsehtml(ph);
    return 1;
}
endctOP() 

ctOP(ctUnaryfunc)
{
    if(which==ATTRIB){
        pHtmlNode pHtc= curHtml->createHtmlNode(tagid,attrib);
        pHtmlNode pHtp = curHtml->_3dhtop();
        HNinsert(pHtp,pHtc);
        return 1;
    }
}
endctOP() 


ctOP(ctShtmlfunc)
{
    if(which==ATTRIB){
        void * tmpAttrp = (void*)malloc(sizeof(attrBase_t));
        memset(tmpAttrp,0,sizeof(attrBase_t));
        curHtml->_3dhpush(tagid,tmpAttrp);
        pHtmlNode pHtc=curHtml-> _3dhtop();
        curHtml->phead=pHtc;
    }
    else if(which==VALUE){
        insertTextNode(curHtml, value);
    }
    return 1;
}
endctOP() 


ctOP(ctEhtmlfunc)
{
    //curHtml->phead=curHtml->_3dhpop();
}
endctOP() 

ctOP(ctStablefunc)
{
    if(which==ATTRIB){
        callBrfunc(curHtml); 
        pHtmlNode pHtp = curHtml->_3dhtop();
        pHtmlNode tmpNode= new HtmlTableNode(pHtp , tp);
        curHtml->_3dhpush(tmpNode);

    }
    else if(which==VALUE){
        insertTextNode(curHtml, value);
    }
    return 1;
}
endctOP() 

ctOP(ctEtablefunc)
{
    if( curHtml->_stackisSpouse(tagid)){	
        pHtmlNode pHtc = curHtml->_3dhpop();
        pHtc -> endParse();
        return 1;
    }
    else{
        while(! curHtml->_stackisSpouse(tagid)){
            int tag = curHtml->gettagfromstack() + 1;
            dmToken t(tag, NULL,NULL);
            (curHtml->ctTagFunc[tag])(curHtml,&t,ATTRIB);
        }
        //ctEtablefunc( curHtml,tagid,NULL,ATTRIB,NULL);
        dmToken t(tagid, NULL, NULL);
        ctEtablefunc( curHtml, &t, ATTRIB);
    }
    return 0;
}
endctOP() 

ctOP(ctStrfunc)
{
    if(which==ATTRIB){
        pHtmlNode pHtp = curHtml->_3dhtop();
        pHtmlNode tmpNode= new HtmlTrNode(pHtp , tp);
        curHtml->_3dhpush(tmpNode);
    }
    else if(which==VALUE){
        insertTextNode(curHtml, value);
    }
    return 1;
}
endctOP() 

ctOP(ctEtrfunc)
{
    if( curHtml->_stackisSpouse(tagid)){	
        pHtmlNode pHtc = curHtml->_3dhpop();
        pHtc -> endParse();
        return 1;
    }
    return 0;
}
endctOP() 

ctOP(ctStdfunc)
{
    if(which==ATTRIB){
        int prvtag = curHtml->gettagfromstack();
        debprintf("prv tag: %d, curtag:%d\n",prvtag,tagid);
        if (prvtag == TDID) {
            dmToken t(TDID+1, NULL, NULL);
            curHtml->ctEtdfunc(curHtml,&t,ATTRIB);
        }
        pHtmlNode pTrNode = curHtml->_3dhtop();
        pHtmlNode tmpNode= new HtmlTdNode(pTrNode , tp);
        curHtml->_3dhpush(tmpNode);

    }
    else if(which==VALUE){
        extern int charWidth;
        insertTextNode(curHtml, value);
    }
    return 1;
}
endctOP() 

ctOP(ctEtdfunc)
{
    if( curHtml->_stackisSpouse(tagid)){	
        pHtmlNode pHtc = curHtml->_3dhpop();
        pHtc -> endParse();
        return 1;
    }
    return 0;
}
endctOP() 


ctOP(ctSulfunc)
{
    if(which==ATTRIB){
        pHtmlNode pHtp = curHtml->_3dhtop();
        pHtmlNode tmpNode= new HtmlUlNode(pHtp, tp);
        curHtml->_3dhpush(tmpNode);

    }
    else if(which==VALUE){
        insertTextNode(curHtml, value);
    }
    return 1;
}
endctOP() 

ctOP(ctEulfunc)
{
    if( curHtml->_stackisSpouse(tagid)){	

        pHtmlNode pHtc = curHtml->_3dhpop();
        pHtc -> endParse();
        return 1;
    }
    return 0;
}
endctOP() 

ctOP(ctImgfunc)
{
        pHtmlNode pHtp = curHtml->_3dhtop();
        pHtmlNode tmpNode= new HtmlImgNode(pHtp, tp);
     return 1;
}
endctOP() 

ctOP(ctBrfunc)
{
        pHtmlNode pHtp = curHtml->_3dhtop();
        pHtmlNode tmpNode= new HtmlBrNode(pHtp, tp);

    return 1;
}
endctOP() 

ctOP(ctInputfunc)
{
        pHtmlNode pHtp = curHtml->_3dhtop();
        pHtmlNode tmpNode= new HtmlInputNode(pHtp, tp);
    return 1;
}
endctOP() 

ctOP(ctSafunc)
{
    extern int getCharsWidth(char *s);
    extern int2 getstrWH(char *);
    if(which==ATTRIB){
        pHtmlNode pHtp = curHtml->_3dhtop();
        pHtmlNode tmpNode= new HtmlANode(pHtp, tp);
        curHtml->_3dhpush(tmpNode);
    }
    else if(which==VALUE){
        insertTextNode(curHtml, value);
    } else if(which == TAGCOMPLETE){
        pHtmlNode pHtp = curHtml->_3dhtop();
        pHtmlNode tmpNode= new HtmlSimpleANode(pHtp, tp);
    }
    return 1;
}
endctOP() 

ctOP(ctEafunc)
{
    if( curHtml->_stackisSpouse(tagid)){	

        pHtmlNode pHtc = curHtml->_3dhpop();
        pHtc -> endParse();
        return 1;

    }
    return 0;
}
endctOP() 

ctOP(ctSpfunc)
{
    if(which==ATTRIB){
        pHtmlNode pHtp = curHtml->_3dhtop();
        pHtmlNode tmpNode= new HtmlPNode(pHtp, tp);
        curHtml->_3dhpush(tmpNode);
    }
    else if(which==VALUE){
        insertTextNode(curHtml, value);
    }
    return 1;
}
endctOP() 

ctOP(ctEpfunc)
{
    if( curHtml->_stackisSpouse(tagid)){	
        pHtmlNode pHtc = curHtml->_3dhpop();
        pHtc -> endParse();
        return 1;
    }
    return 0;
}
endctOP() 
    extern void getRect(int *);
ctOP(ctSdivfunc)
{
    if(which==ATTRIB){
        callBrfunc(curHtml); 
        pHtmlNode pHtp = curHtml->_3dhtop();
        pHtmlNode tmpNode= new HtmlDivNode(pHtp, tp);
        curHtml->_3dhpush(tmpNode);
    }
    else if(which==VALUE){
        insertTextNode(curHtml, value);
    }
    return 1;
}
endctOP() 

ctOP(ctEdivfunc)
{
    if( curHtml->_stackisSpouse(tagid)){	
        pHtmlNode pHtc = curHtml->_3dhpop();
        pHtc -> endParse();
        return 1;
    }
    return 0;
}
endctOP() 

ctOP(ctSstylefunc)
{
    if(which==ATTRIB){
    }
    else if(which==VALUE){
        int parseCss(char *attrstr,void ** pobj);
        void printcssID();
        parseCss(value,NULL);
        printcssID();
        debprintf(" %s\n",value);
        free(value);
    }
}
endctOP() 

ctOP(ctEstylefunc)
{
}
endctOP() 

ctOP(ctSspanfunc)
{
    // 2008-5-24 skip it
    void printRect();
    printRect();
    SKIPTHISTAG
        return 1;
    if(which==ATTRIB){
        spanattr_t *pspanattr =(spanattr_t*) malloc(sizeof(spanattr_t)); 
        setZero(pspanattr,spanattr_t);
        curHtml->_3dhpush(tagid,(void*)pspanattr);
    }
    else if(which==VALUE){
    } 
}
endctOP() 

ctOP(ctEspanfunc)
{
    return 1;
}
endctOP() 


ctOP(ctSformfunc)
{
    // 2008-5-24 skip it
    return 1;
}
endctOP() 

ctOP(ctEformfunc)
{
    return 1;
}
endctOP() 

ctOP(ctSfontfunc)
{
    // 2008-5-24 skip it
    SKIPTHISTAG
        return 1;
}
endctOP() 

ctOP(ctEfontfunc)
{
    return 1;
}
endctOP() 

ctOP(ctSbfunc)
{
    // 2008-5-24 skip it
    int curtag = curHtml->gettagfromstack();
    if(which==ATTRIB){
    }
    else if(which==VALUE){
        dmToken upT(curtag, NULL,tp->value);
        (curHtml->ctTagFunc[curtag])(curHtml,&upT, VALUE);
    }
    return 1;
}
endctOP() 

ctOP(ctEbfunc)
{
    return 1;
}
endctOP() 

ctOP(ctScenterfunc)
{
#if 1
    if(which==ATTRIB){
        callBrfunc(curHtml); 
        pHtmlNode pHtp = curHtml->_3dhtop();
        pHtmlNode tmpNode= new HtmlCenterNode(pHtp, tp);
        curHtml->_3dhpush(tmpNode);
    }
    else if(which==VALUE){
        insertTextNode(curHtml, value);
    }
#endif
    return 1;
}
endctOP() 

ctOP(ctEcenterfunc)
{
    //return 1;
    if( curHtml->_stackisSpouse(tagid)){	
        pHtmlNode pHtc = curHtml->_3dhpop();
        pHtc -> endParse();
    }
    //#endif
}
endctOP() 

ctOP(ctSnobrfunc)
{
    // 2008-5-24 skip it
    SKIPTHISTAG
        return 1;
}
endctOP() 

ctOP(ctEnobrfunc)
{
    return 1;
}
endctOP() 

ctOP(ctSufunc)
{
    // 2008-5-24 skip it
    SKIPTHISTAG
        return 1;
}
endctOP() 

ctOP(ctEufunc)
{
    return 1;
}
endctOP() 

ctOP(ctSsmallfunc)
{
    // 2008-5-24 skip it
    SKIPTHISTAG
        return 1;
}
endctOP() 

ctOP(ctEsmallfunc)
{
    return 1;
}
endctOP() 

ctOP(ctSheadfunc)
{
    // 2008-5-24 skip it
    return 1;
}
endctOP() 

ctOP(ctEheadfunc)
{
    return 1;
}
endctOP() 

ctOP(ctStitlefunc)
{
    // 2008-5-24 skip it
    return 1;
}
endctOP() 

ctOP(ctEtitlefunc)
{
    return 1;
}
endctOP() 

ctOP(ctSskipfunc)
{
    // 2008-5-24 skip it
    SKIPTHISTAG
        return 1;
}
endctOP() 

ctOP(ctEskipfunc)
{
    return 1;
}
endctOP() 

