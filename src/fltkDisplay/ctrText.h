#ifndef CTRTEXT_HEADER
#define CTRTEXT_HEADER
#ifndef INLINE
#define INLINE // inline 
#endif
#include <stdlib.h>
#include <stdio.h>
/*
#include <FL/Fl.H>
#include <FL/Widget.H>
#include <FL/Single_Window.H>
#include <FL/fl_draw.H>
*/
#include <fltk/Output.h>
#include <fltk/run.h>
#include <fltk/Window.h>
#include <fltk/InvisibleBox.h>
#include "cssBox.h"
using namespace fltk;
#include "display.h"
class ctrText : public Output {
    public:
    int startx;
    void draw();
    ctrText(int w,int h,const char *l)
    : Output(0,0,w,h,0)
    {type(5);textfont(TIMES);textsize(14);value(l);}

    ctrText(int x,int y,int w,int h,const char *l) 
    : Output(x,y,w,h,0)
    {type(5);textfont(TIMES);textsize(14);value(l);}
};

#endif
