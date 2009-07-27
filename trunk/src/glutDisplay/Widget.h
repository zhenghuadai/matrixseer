#ifndef __WIDGET_HEADER__
#define __WIDGET_HEADER__
#include "Cube.h"
#include "Box.h"
#include "dmtype.h"
#include "debprintf.h"
class Widget: public Cube
{
friend class Group;
protected:
	Widget* mParent;
	Widget* mNext;
	Widget* mPrevious;
public:
	Widget(){init();};
	Widget(int x,int y,int z):Cube(x,y,z){init();};
	Widget(int dx,int dy):Cube(dx,dy){init();};
	Widget(int x,int y,int z,int dx,int dy,const char* l):Cube(x,y,z,dx,dy){init();};
	Widget(int x,int y,int dx,int dy , const char* l):Cube(x,y,0,dx,dy){init();};
	void box(Box* b){}
	void box(int b){}


	void show(){};
	void redraw(){};
    void begin(){};
    void end(){};
    void color(int){};
    void labelcolor(int){};
	void setParent(Widget* o){ mParent = o;}
	void setNext(Widget* o);
	Widget* parent(){return mParent;}
	void parent(Widget* o) { mParent = o;}
	Widget* next() { return mNext;}
	void next(Widget* o){ mNext = o;}
	virtual	void draw(){ debprintf("Widget draw\n");};
	virtual void addChild(Widget*){};
	int getSx();
	int getSy();
	int getSz();
	int3 getSxyz();
	void getSxyz(int& sx, int& sy, int& sz);
private:
    void init(){mParent = mNext = mPrevious = NULL;}
};
#endif
