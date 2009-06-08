#ifndef __CTRBUTTON_HEADER__
#define __CTRBUTTON_HEADER__
#ifndef CTRBINLINE
#define CTRBINLINE // inline 
#endif
#include "Widget.h"
class ctrButton: public Widget
{
public:
 ctrButton();
 ctrButton(int sdx,int sdy);
 ctrButton(int sdx,int sdy,char *s);
 ctrButton(int sx,int sy, int sz);
 ctrButton(int sx,int sy, int sz,int sdx,int sdy);
 ctrButton(int sx,int sy,int sz,char * s);
 void OnClick();
~ctrButton();
void draw();
void redraw();
void keyfunc(char  key,int x,int y);
private:
char *pvalue;
int curc;
};
#endif
