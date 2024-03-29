#ifndef CTRTEXT_HEADER
#define CTRTEXT_HEADER
#ifndef INLINE
#define INLINE // inline 
#endif
#include <stdlib.h>
#include <stdio.h>
//#include "cssBox.h"
#include "display.h"
#include "Output.h"
class ctrText : public Output {
    public:
    int startx;
    ctrText(int w,int h,const char *l)
    : Output(0,0,w,h,l)
    {type(5);textfont(TIMES);textsize(14);}

    ctrText(int x,int y,int w,int h,const char *l) 
    : Output(x,y,w,h,l)
    {type(5);textfont(TIMES);textsize(14);debprintf("this ->%0x, text:%s\n", this, value());
    }

    ctrText(int x,int y,int z, int w,int h, int d, const char *l) 
    : Output(x,y,z,w,h,d,l)
    {type(5);textfont(TIMES);textsize(14);    }
};

#endif
