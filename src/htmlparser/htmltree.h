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
class HtmlNode;
void  updateParentHtmlNode(HtmlNode *pHtp, HtmlNode* pHtc);
typedef void* attr_t;
class HtmlNode{
    public:
        int tagid;
        void * attr;
        char * value;
        void * obj;
        int wife;
        HtmlNode * child;
        HtmlNode * mPrevious;
        HtmlNode * mNext;
        HtmlNode * mParent;
    public:
        HtmlNode(int _tag):tagid(_tag),obj(NULL),attr(NULL),value(NULL),child(NULL),mPrevious(NULL),mNext(NULL),mParent(NULL){}
        HtmlNode(HtmlNode* pHtp, int _tag):tagid(_tag),obj(NULL),attr(NULL),value(NULL),child(NULL),mPrevious(NULL),mNext(NULL)/*,mParent(pHtp)*/{ pHtp -> addChild(this);}
        HtmlNode():tagid(-1),obj(NULL),attr(NULL),value(NULL),child(NULL),mPrevious(NULL),mNext(NULL),mParent(NULL){}
    private:
        void setNext(HtmlNode* o){
            this -> mNext = o;
            if(o)
                o -> mPrevious = this;
        }

        void appandChild(HtmlNode* child_){
            HtmlNode* lastSib = child->mPrevious;
            lastSib -> setNext(child_);
            child -> mPrevious = child_;
            child_ -> mNext = 0;

            child_->mParent = this;
        }
        void addFirstChild(HtmlNode* child_){ child = child_; child_->mParent = this; child_->mPrevious= child_;}
    public:
        void addChild(HtmlNode* child_){
            if(child) {
                appandChild(child_);
            }else{
                addFirstChild(child_);
            }
        }
        // the follow is for parsing
        HtmlNode* parent(){return mParent;} 
    public:
        Widget* getRenderObject(){ return (Widget*) obj;}
        Widget* getParentRenderObject(){ 
            HtmlNode *p = parent();
            while((p)&&( p ->obj == NULL)) { p = p ->parent();}
            if(p) 
                return (Widget*) p->obj;
            else 
                return NULL;
        }
        void createObject(){
            createRenderObject();
            if (getRenderObject() ==NULL)return;
#if NEED_RENDER == 1
             getRenderObject()->htmlNode(this);
            Widget* pobj = 	getParentRenderObject();
            if(pobj ) 
                pobj -> addChild(getRenderObject());
#endif
        }
        void finishParse(){ endParse(); updateParentHtmlNode(parent(),this);}
    public:

        virtual void endParse() {} ; // = 0;
        virtual void processValue(dmToken*){};// = 0;
        virtual void createRenderObject(){};// = 0;
};

typedef HtmlNode* pHtmlNode;

#define declareHtmlNode(className) \
    class className:public HtmlNode\
{\
    public: \
            className(pHtmlNode pHtp, dmToken* t);\
    className(){};\
    public:\
           virtual void   endParse();\
    virtual void   processValue(dmToken * t);\
    virtual void createRenderObject();\
    public:\
};
#define declareHtmlUnaryNode(className) \
    class className:public HtmlNode\
{\
    public: \
            className(pHtmlNode pHtp, dmToken* t);\
    public:\
           virtual void   endParse(){};\
    virtual void   processValue(dmToken * t){};\
    virtual void createRenderObject();\
    public:\
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
class HtmlBodyNode:public HtmlDivNode 
{
    public: 
        HtmlBodyNode(pHtmlNode pHtp, dmToken* t);
};

class htmlDocument:public HtmlNode 
{
    private:
        void* bodyRenderObj;
    public: 
        htmlDocument(dmToken* t);
        void* getBodyRenderObj(){}
    public:
        virtual void endParse() {} ; 
};

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

    if(pHtp && pHtc && (pHtp -> attr )&&(pHtc -> attr))
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
