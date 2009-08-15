#ifndef CTRP_HEADER
#define CTRP_HEADER
#ifndef INLINE
#define INLINE // inline 
#endif
#include <stdlib.h>
#include <stdio.h>
//#include "cssBox.h"
#include "display.h"
#include "postionFlag.h"
#include "Group.h"
class ctrP : public Group {
    //public:
    //int PostionFlag;
    declarePFlag
    private:
    int handle(int);
    int pos[4];
    void initP(){PostionFlag = 0;box(CSS_BOX/*BORDER_FRAME*/);};
    public:
    int pushed ;
    void draw(){};
    //  ctrP():Box(0,0,0,0,""){};
    ctrP(int x,int y,int flag)
    : Group(x,y,0,0,(const char *)0) 
    {pushed = 0;initP();PostionFlag=flag;};

    ctrP(Box* b,int w,int h,const char *l)
        : Group(0,0,w,h,l) 
        {box(b);pushed = 0;initP();};
/*
    ctrP(int w,int h,const char *l)
    : Group(0,0,w,h,l)
    {pushed = 0;initP();}
*/
    ctrP(int x,int y,int w,int h,const char *l) 
    : Group(x,y,w,h,l) {}
    ctrP(Box* b,int x,int y,int w,int h,const char *l)
        : Group(x,y,w,h,l) {}
    posFlagFunc
    /*
    void setPostionFlag(int flag){PostionFlag = flag;    }
    void setFixX(int x0){x(x0);PostionFlag |= FIXEDX;}
    void setFixY(int y0){y(y0);PostionFlag |= FIXEDY;}
    void setFixXY(int x,int y){position(x,y);PostionFlag |= FIXEDPOS;}
    void setFixW(int w0){w(w0);PostionFlag |= FIXEDW;}
    void setFixH(int h0){h(h0);PostionFlag |= FIXEDH;}
    void setFixWH(int w,int h){size(w,h);PostionFlag |= FIXEDSIZE;}
    */
    void StartR();
    void EndR();
};

#endif
