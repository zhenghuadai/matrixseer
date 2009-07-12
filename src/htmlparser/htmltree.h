#ifndef HTMLTREE_HEADER
#define HTMLTREE_HEADER
/*
   typedef struct {
   void * value;
   void * pdraw;
#define pinner
} attr_t,* pattr_t;
*/
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

#include "token.h"
#include "htmltag.h"
#define setZero(addr,type)\
    memset(addr,0,sizeof(type));

#define ALIGNLEFT 0
#define ALIGNRIGHT 1
#define ALIGNCENTER 2
#define attrOfHtmlNode( pNode) pNode -> attr
typedef void* attr_t;
class HtmlNode{
    public:
        int tagid;
        void * attr;
        char * value;
        void * obj;
        int wife;
        HtmlNode * child;
        HtmlNode * sib;
        HtmlNode * lastsib;
        HtmlNode * parent;
    private:
        void appandChild(HtmlNode* child_){
            HtmlNode* sil1st=child;
            if(sil1st->sib){
            // 
                sil1st->lastsib->sib = child_;
                sil1st->lastsib = child_;
            }
            else{
            // the first sib
                sil1st->sib=child_;
                sil1st->lastsib=child_;
            }
            child_->parent = this;
        }
        void addFirstChild(HtmlNode* child_){ child = child_; child_->parent = this;}
    public:
        HtmlNode():child(NULL),sib(NULL),lastsib(NULL),parent(NULL){}
    public:
        void addChild(HtmlNode* child_){
            if(child) {
                appandChild(child_);
            }else{
                addFirstChild(child_);
            }
        }
        // the follow is for parsing
    public:
        virtual void endParse() = 0;
        virtual void processValue(dmToken*) = 0;
};

typedef HtmlNode* pHtmlNode;

#define declareHtmlNode(className) \
class className:public HtmlNode\
{\
    public: \
        className(pHtmlNode pHtp, dmToken* t);\
    public:\
        void   endParse();\
        void   processValue(dmToken * t);\
    public:\
        void createObject();\
};
#define declareHtmlUnaryNode(className) \
class className:public HtmlNode\
{\
    public: \
        className(pHtmlNode pHtp, dmToken* t);\
    public:\
        void   endParse(){};\
        void   processValue(dmToken * t){};\
    public:\
        void createObject();\
};

/* example
class HtmlUlNode:public HtmlNode
{
    public: 
        HtmlUlNode(pHtmlNode pHtp, dmToken* t);
    public:
        void   endParse();
        void   processValue(dmToken * t);
    public:
        void createObject();
};
*/
declareHtmlNode(HtmlUlNode);
declareHtmlNode(HtmlTableNode);
declareHtmlNode(HtmlTrNode);
declareHtmlNode(HtmlTdNode);
declareHtmlNode(HtmlPNode);
declareHtmlNode(HtmlANode);
declareHtmlNode(HtmlSimpleANode);
declareHtmlNode(HtmlDivNode);
declareHtmlNode(HtmlCenterNode);
declareHtmlUnaryNode(HtmlInputNode);
declareHtmlUnaryNode(HtmlImgNode);
declareHtmlUnaryNode(HtmlBrNode);

class attrBase_t
{
    public:
        int startx;
        int starty;
        int w;
        int h;
        int * basex;
        int * basey;
        int curW;
        int curH;
        int curX;
        int curY;
        char * p_id;
        char * p_class;
        int bgcolor;
        int align;
    public:
        attrBase_t(){}
};

#define objBase attrBase_t

typedef struct{
    int length;
    int offset;
    int realOffset;
} gridPos_t;

class tableattr_t :public attrBase_t{
    public:
        int cols;
        int rows;
        gridPos_t colsW[50];
        gridPos_t rowsH[50];
} ;

class trattr_t:public attrBase_t{
    public:
        int cols;
        gridPos_t *colsMaxW;
        gridPos_t *currentRowH;
        int rowID;
};

class tdattr_t:public attrBase_t{
    public:
        int *colsMaxW;
        int *pstartx; 
        int *pstarty;
        int colID;
};

class imgattr_t:public attrBase_t{
    public:
        char *src;
        char *alt;
};
#define INPUTINPUT 0
#define INPUTBUTTON 1
#define INPUTRAIDO 2
#define INPUTUNKNOW 5 
class  inputattr_t:public attrBase_t{
    public:
        char *name;
        int type;
        char *value;
        int size;
        int maxlength;
        char * title;
};

class aattr_t:public attrBase_t{
    public:
        char *href;
} ;

class pTAGattr_t:public attrBase_t{
} ;

class divattr_t:public attrBase_t{
} ;

class  spanattr_t:public attrBase_t{
} ;

class  ulattr_t:public attrBase_t{
} ;

inline attrBase_t * getAttrBaseP(pHtmlNode pHtc)
{
    return ((attrBase_t*) (pHtc -> attr));
};

inline void GetCurXYFromPhtmlNode(int &x,int &y,pHtmlNode pHtp)
{
    attrBase_t * pAttrp = (attrBase_t*)pHtp -> attr;
    x = pAttrp -> curW;
    y = pAttrp -> h;
    if(x<0) x=0;
    if(y<0) y=0;
}

inline void GetCurXYFromPattr(int &x,int &y,attrBase_t* pAttrp)
{
    x = pAttrp -> curW;
    y = pAttrp -> h;
    if(x<0) x=0;
    if(y<0) y=0;
}

inline void updateSelfAttrEnter(attrBase_t * pAttrc)
{
    pAttrc -> h += pAttrc -> curH;
    if(pAttrc -> w < pAttrc->curW) pAttrc->w = pAttrc -> curW;
}
inline void  updateParentHtmlNode(pHtmlNode pHtp, pHtmlNode pHtc)
{
    if((pHtp -> attr )&&(pHtc -> attr))
    {
        attrBase_t *pAttrp = getAttrBaseP(pHtp);
        attrBase_t *pAttrc = getAttrBaseP(pHtc);
        debprintf("%d",pAttrp->curW);
        pAttrp -> curW += pAttrc ->w;
        if(pAttrc -> h > pAttrp -> curH) pAttrp -> curH = pAttrc -> h;
        debprintf("+%d = %d \n",pAttrc->w,pAttrp->curW);
    }
}


inline void enterAttr(attrBase_t *pAttrp)
{
    pAttrp -> h += (pAttrp -> curH + 5);
    if(pAttrp -> w < pAttrp->curW) pAttrp->w = pAttrp -> curW;
    pAttrp -> curW = pAttrp -> curH = 0;

}

inline void enterHtmlNode(pHtmlNode pHtp)
{
    attrBase_t * pAttrp = getAttrBaseP(pHtp);
    enterAttr(pAttrp);
}

inline void  updateCurWH(pHtmlNode pHtc,int wl,int hl)\
{
    if((pHtc -> attr)){
        attrBase_t * pAttrc = getAttrBaseP(pHtc);
        pAttrc -> curW += wl;
        if(pAttrc -> curH < hl) pAttrc-> curH = hl;
    }
}

inline void updateSelfHtmlNode(pHtmlNode pHtc)
{
    attrBase_t * pAttrc = (attrBase_t*) pHtc -> attr;
    enterAttr(pAttrc);
}

inline void updateParentAttr(attrBase_t *pAttrp,attrBase_t* pAttrc) 
{
    pAttrp -> curW += pAttrc ->w;
    if(pAttrc -> h > pAttrp -> curH) pAttrp -> curH = pAttrc -> h;
}


#define UPDATEatEnd \
    updateSelfHtmlNode(pHtc);\
updateParentHtmlNode(pHtp,pHtc);

inline void GetCurWHFromhtmlNode(int &w,int &h,pHtmlNode pHtp) 
{
    attrBase_t * pAttrp = (attrBase_t*)pHtp -> attr;
    w = pAttrp -> w;
    h = pAttrp -> h;
}

inline void GetCurWHFromPattr(int &w,int &h,attrBase_t* pAttrp) 
{
    w = pAttrp -> w;
    h = pAttrp -> h;
}



#endif
