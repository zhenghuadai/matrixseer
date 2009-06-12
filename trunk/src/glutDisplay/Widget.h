#ifndef __WIDGET_HEADER__
#define __WIDGET_HEADER__
#include "Rect.h"
#include "Box.h"
class Widget: public Rect
{
public:
	Widget(){};
	Widget(int x,int y,int z):Rect(x,y,z){};
	Widget(int dx,int dy):Rect(dx,dy){};
	Widget(int x,int y,int z,int dx,int dy):Rect(x,y,z,dx,dy){};
	Widget(int x,int y,int dx,int dy , const char* l):Rect(x,y,0,dx,dy){};
	void box(Box* b){}
	void box(int b){}


	void show();
	void redraw();
};
#endif
