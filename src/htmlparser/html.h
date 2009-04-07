#ifndef HTMLHEAD_H
#define HTMLHEAD_H
#include <stdio.h>
#include "stack.h"
#include "debprintf.h"
#include "token.h"
#define OParg pHtmlNode htnode
#define OP(a) int a(pHtmlNode htnode)
//#define ctOP(a) static int a(htmlparser*,int tagid,char *htdoc,int which,char *htdoc2=0)
#define ctOP(a) static int a(htmlparser*,dmTokenP tp, int which)
#define ctOP2(a,b) ctOP(a);\
ctOP(b)
#define atOP(a) static int a(char * attrstr, void ** pobj)
class htmlparser:public __stack
{
    public:
        htmlparser();
        ~htmlparser();
        void initialFunc();
        int parsehtml(char *htdoc);
        void renderhtml();
        int lookuptag(char *tag);
        typedef int (htmlparser::*tagfunc_t)(OParg);
        //typedef int (htmlparser::*ctTagFunc_t)(htmlparser*,int tagid,char *htdoc,int which,char *htdoc2);
        //typedef int (*ctTagFunc_t)(htmlparser*,int tagid,char *htdoc,int which,char *htdoc2);
        typedef int (*ctTagFunc_t)(htmlparser*,dmTokenP tp,int which);
        typedef int (htmlparser::*attrFunc_t)(char * attrstr, void ** pobj);
        int addtag(char *tag,int TAGID,tagfunc_t fp);
        int gettag(char *tag);
        int destroy();
    private:
        char * tagbufp;
        tagfunc_t tagfunc[100];
        static  ctTagFunc_t ctTagFunc[100];
        attrFunc_t attrfunc[100];
        pHtmlNode  phead;
    private:
        char *gettagstr(char **pos);
        char *getattrstr(char **pos);
        char *getvaluestr(char **pos,int *nexttag);
        char *getvaluestr(char **pos,int *nexttag,int gototag);
        char *getvaluestrp(char **pos,int *nexttag);
        char *gettagstrfromid(int id);
        int releaseNode(pHtmlNode root);
        int releaseTree(pHtmlNode root);
        void visitHtmlNode(pHtmlNode root);
        void rendernode(pHtmlNode root);
        void _printstack();
        //int processTag(int tagid,char *attributes,int which,char *value=0);
        //int processValueOfTag(int tagid,char *attributes,int which,char *value);
        //int processSimpleTag(int tagid,char *attributes,int which,char *value);
        //int processEndTag(int tagid,char *htdoc,int which,char *htdoc2=0);
        int processTag(dmTokenP tp,int which);
        int processValueOfTag(dmTokenP tp,int which);
        int processSimpleTag(dmTokenP tp,int which);
        int processEndTag(dmTokenP tp,int which);
        //static int ctSfunc(htmlparser*,int tagid,char *htdoc,int which,char *htdoc2=0);
        //static int ctEfunc(htmlparser*,int tagid,char *htdoc,int which,char *htdoc2=0);
        ctOP(ctSfunc);
        ctOP(ctEfunc);
        ctOP(ctShtmlfunc);
        ctOP(ctEhtmlfunc);
        ctOP(ctSscriptfunc);
        ctOP(ctEscriptfunc);

        ctOP(ctUnaryfunc);
        ctOP(ctImgfunc);
        ctOP(ctBrfunc);
        ctOP(ctInputfunc);

        ctOP(ctStablefunc);
        ctOP(ctEtablefunc);
        ctOP(ctStrfunc);
        ctOP(ctEtrfunc);
        ctOP(ctStdfunc);
        ctOP(ctEtdfunc);
        ctOP(ctSulfunc);
        ctOP(ctEulfunc);
        ctOP(ctSafunc);
        ctOP(ctEafunc);
        ctOP(ctSpfunc);
        ctOP(ctEpfunc);
        ctOP(ctSdivfunc);
        ctOP(ctEdivfunc);
        ctOP(ctSstylefunc);
        ctOP(ctEstylefunc);
        ctOP(ctSspanfunc);
        ctOP(ctEspanfunc);
        ctOP(ctSformfunc);
        ctOP(ctEformfunc);
        ctOP(ctSfontfunc);
        ctOP(ctEfontfunc);
        ctOP(ctSbfunc);
        ctOP(ctEbfunc);
        ctOP(ctSufunc);
        ctOP(ctEufunc);
        ctOP(ctScenterfunc);
        ctOP(ctEcenterfunc);
        ctOP(ctSnobrfunc);
        ctOP(ctEnobrfunc);
        ctOP(ctSsmallfunc);
        ctOP(ctEsmallfunc);
        ctOP2(ctSheadfunc,ctEheadfunc);
        ctOP2(ctStitlefunc,ctEtitlefunc);
        ctOP2(ctSskipfunc,ctEskipfunc);
        int ctTextFunc(pHtmlNode pHtp, char * htdoc);


        OP(Shtmlfunc);
        OP(Ehtmlfunc);
        OP(Sheadfunc);
        OP(Eheadfunc);
        OP(Sscriptfunc);
        OP(Escriptfunc);
        OP(Sbodyfunc);
        OP(Ebodyfunc);
        OP(Spfunc);
        OP(Epfunc);
        OP(brfunc);
        OP(afunc);


        OP(Stablefunc);
        OP(Etablefunc);
        OP(Strfunc);
        OP(Etrfunc);
        OP(Stdfunc);
        OP(Etdfunc);
        OP(Sdivfunc);
        OP(Edivfunc);
        OP(Sulfunc);
        OP(Eulfunc);
        OP(Slifunc);
        OP(Elifunc);
        OP(Sfontfunc);
        OP(Efontfunc);
        OP(imgfunc);
        OP(Sh1func);
        OP(Eh1func);
        OP(Sh2func);
        OP(Eh2func);
        OP(Sh3func);
        OP(Eh3func);
        OP(Sh4func);
        OP(Eh4func);
        OP(Sh5func);
        OP(Eh5func);
        OP(metafunc);
        OP(linkfunc);
        OP(Stitlefunc);
        OP(Etitlefunc);
        OP(Sstylefunc);
        OP(Estylefunc);
        OP(Sgeneralfunc);
        OP(Egeneralfunc);
        OP(Safunc);
        OP(Eafunc);
        OP(SgetErrtag);
        OP(Evaluefunc);
        OP(inputfunc);
public:
        atOP(ImgAttr);
        atOP(tableAttrFunc);
        atOP(tdAttrFunc);
        atOP(GeneralAttr);
        atOP(InputAttr);
        atOP(aAttr);
        atOP(pAttr);
        atOP(DivAttr);
};
#include "htmltag.h"
#endif
