#ifndef __WIDGET_HEADER__
#define __WIDGET_HEADER__
#include "Rect.h"
class Widget: public Rect
{
	Widget(){};
	Widget(int x,int y,int z):Rect(x,y,z){};
	Widget(int dx,int dy):Rect(dx,dy){};
	Widget(int x,int y,int z,int dx,int dy):Rect(x,y,z,dx,dy){};
};
#endif
