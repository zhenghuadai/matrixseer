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
    : Output(0,0,w,h,0)
    {type(5);textfont(TIMES);textsize(14);value(l);}

    ctrText(int x,int y,int w,int h,const char *l) 
    : Output(x,y,w,h,0)
    {type(5);textfont(TIMES);textsize(14);value(l); printf("text:%s\n", value());
    }
};

#endif
