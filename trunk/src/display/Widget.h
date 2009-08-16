#ifndef __WIDGET_HEADER__
#define __WIDGET_HEADER__
#include "EventTarget.h"
#include "Box.h"
#include "dmtype.h"
#include "debprintf.h"
#include "Event.h"
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
	int hitMe(int xi, int yi, int zi) { int sx,sy,sz; getSxyz(sx,sy, sz); 
		int ret= isPinRect(xi, yi, sx, sy, w(), h()); 
		printf("%d %d hitMe test %0x  %d %d %d %d == %d\n", xi, yi, this, sx, sy, w(), h(), ret);
        return ret;
	}	
	virtual Widget* getObjOnFocus(int x, int y, int z){return this;}
	virtual int handleKey(Event e){};
	virtual int handleKey(int key){};
	virtual void redraw(){};
	//!
public:
	void getLBpos(int& leftX, int& bottomY, int& z){getSxyz(leftX, bottomY, z); bottomY -= h(); }
private:
	void init(){mParent = mNext = mPrevious = NULL;}
};
#endif