#ifndef __CTRBUTTON_HEADER__
#define __CTRBUTTON_HEADER__
#ifndef CTRBINLINE
#define CTRBINLINE // inline 
#endif
#include "Widget.h"
class ctrButton: public Widget
{
private:
char *pvalue;
int state;
public:
 ctrButton();
 ctrButton(int sdx,int sdy);
 ctrButton(int sdx,int sdy,char *s);
 ctrButton(int sx,int sy, int sz);
 ctrButton(int sx,int sy, int sz,int sdx,int sdy);
 ctrButton(int sx,int sy,int sz,char * s);
 ctrButton(int x, int y, int w, int h, char* l);
 ctrButton(int x, int y, int z, int w, int h, int d, char* l);
 void OnClick();
~ctrButton();
void draw();
void redraw();
virtual int handleKey( int key) { keyfunc(key, 0,0); return 1;}
virtual int handleButton(ButtonEvent e){ OnClick();}
private:
void keyfunc(int key,int x,int y);
};
#endif
