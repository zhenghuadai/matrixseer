#ifndef __WIDGET_HEADER__
#define __WIDGET_HEADER__
#include "Cube.h"
#include "Box.h"
class Widget: public Cube
{
friend class Group;
protected:
	Widget* mParent;
	Widget* mNext;
	Widget* mPrevious;
public:
	Widget(){};
	Widget(int x,int y,int z):Cube(x,y,z){};
	Widget(int dx,int dy):Cube(dx,dy){};
	Widget(int x,int y,int z,int dx,int dy):Cube(x,y,z,dx,dy){};
	Widget(int x,int y,int dx,int dy , const char* l):Cube(x,y,0,dx,dy){};
	void box(Box* b){}
	void box(int b){}


	void show(){};
	void redraw(){};
    void begin(){};
    void end(){};
    void color(int){};
    void labelcolor(int){};
	virtual	void draw(){ printf("Widget draw\n");};
	void setParent(Widget* o){ mParent = o;}
	void setNext(Widget* o);
private:
};
#endif
