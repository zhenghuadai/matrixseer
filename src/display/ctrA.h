#ifndef CTRA_HEADER
#define CTRA_HEADER
#ifndef INLINE
#define INLINE // inline 
#endif
#include <stdlib.h>
#include <stdio.h>
#include "dmGlobal.h"
#include "Box.h"
#include "ctrText.h"
class ctrA : public ctrText{
    public:
        int handle(int);
    public:
        int pushed ;
        ctrA(Box* b,int w,int h,const char *l)
            : ctrText(0,0,w,h,l) 
        {box(b);pushed = 0;};

        ctrA(int w,int h,const char *l)
            : ctrText(0,0,w,h,l)
        {box(BOX_BODER_SHAPE);pushed = 0;}

        ctrA(int w,int h)
            :ctrText (0,0,w,h,(const char *)0) 
        {box(BOX_BODER_SHAPE);pushed = 0;}

        ctrA(int x,int y,int w,int h,const char *l)
            : ctrText(x,y,w,h,l) 
        {box(BOX_BODER_SHAPE);pushed = 0; 
        }

        ctrA(Box* b,int x,int y,int w,int h,const char *l)
            : ctrText(x,y,w,h,l) 
        {}
};

#endif
