#ifndef __WIDGET_HEADER__
#define __WIDGET_HEADER__
#include "EventTarget.h"
#include "Box.h"
#include "dmtype.h"
#include "debprintf.h"
class Widget: public EventTarget 
{
friend class Group;
protected:
	Widget* mParent;
	Widget* mNext;
	Widget* mPrevious;
public:
	Widget(){init();};
	Widget(int x,int y,int z):EventTarget(x,y,z){init();};
	Widget(int dx,int dy):EventTarget(dx,dy){init();};
	Widget(int x,int y,int z,int dx,int dy,const char* l):EventTarget(x,y,z,dx,dy,l){init();};
	Widget(int x,int y,int dx,int dy , const char* l):EventTarget(x,y,dx,dy,l){init();};
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
	//!Event handler
	int hitMe(int xi, int yi, int zi) { int sx,sy,sz; getSxyz(sx,sy, sz); return isPinRect(xi, yi, sx, sy, w(), h());}	
	Widget* Group::getObjOnFocus(int x, int y, int z){return this;}
	//!
private:
    void init(){mParent = mNext = mPrevious = NULL;}
};
#endif
