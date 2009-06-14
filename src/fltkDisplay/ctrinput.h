#ifndef CTRINPUT_HEADER
#define CTRINPUT_HEADER
#ifndef INLINE
#define INLINE // inline 
#endif
#include <stdlib.h>
#include <stdio.h>
/*
#include <FL/Fl.H>
#include <FL/Input.H>
#include <FL/Toggle_Button.H>
#include <FL/Menu_Button.H>
#include <FL/Box.H>
#include <FL/Input.H>
*/
#include <fltk/run.h>
#include <fltk/Window.h>
#include <fltk/Input.h>
#include <fltk/FloatInput.h>
#include <fltk/IntInput.h>
#include <fltk/SecretInput.h>
#include <fltk/WordwrapInput.h>
#include <fltk/TextEditor.h>
#include <fltk/Button.h>
#include <fltk/ToggleButton.h>
#include <fltk/show_colormap.h>

//#include "common.h"
using namespace fltk;

class ctrInput : public Input {
    int handle(int);
    public:
    void draw();
    ctrInput():Input(0,0,0,0,""){};
    ctrInput(int x,int y,const char *l)
        : Input(x,y,0,0,l) {}
    ctrInput(int w,int h)
        : Input(0,0,w,h,(const char *)0) {}
    ctrInput(int x,int y,int w,int h,const char *l)
        : Input(x,y,w,h,l) {}
    void setxyz(int x,int y, int z=0){position(x,y);}
    void setxyz(){
        int pos[4];
        extern void getCurrentRasterPos(int *pos);
        extern void forwardx(int x); 
        getCurrentRasterPos(pos);
       Widget::position(pos[0],pos[1]);
       forwardx(Widget::w());
//       Widget::position
    }

};

#endif
