#ifndef CTRBUTTON_HEADER
#define CTRBUTTON_HEADER
#ifndef INLINE
#define INLINE // inline 
#endif
#include <stdlib.h>
#include <stdio.h>
/*
#include <FL/Fl.H>
#include <FL/Button.H>
#include <FL/Toggle_Button.H>
#include <FL/Menu_Button.H>
#include <FL/Box.H>
#include <FL/Button.H>
*/
#include <fltk/run.h>
#include <fltk/Window.h>
#include <fltk/Button.h>
#include <fltk/InvisibleBox.h>
#include <fltk/TiledImage.h>
#include <fltk/SharedImage.h>
using namespace fltk;
class ctrButton : public Button {
    int handle(int);
    public:
    void draw();
    ctrButton():Button(0,0,0,0,""){};
    ctrButton(int w,int h,const char *l)
        : Button(0,0,w,h,l) {}
    ctrButton(int x,int y,int w,int h,const char *l)
        : Button(x,y,w,h,l) {}
    void setxyz(int x,int y, int z=0){position(x,y);}
    void setxyz(){
        int pos[4];
        extern void getCurrentRasterPos(int *pos);
        extern void forwardx(int x);
        getCurrentRasterPos(pos);
       Button::position(pos[0],pos[1]);
       forwardx(w());
    }
};

#endif
