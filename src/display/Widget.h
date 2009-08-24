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
	Widget(int w,int h):EventTarget(w,h){init();};
	Widget(int x,int y,int z,int w,int h,const char* l):EventTarget(x,y,z,w,h){init();};
	Widget(int x,int y,int w,int h , const char* l):EventTarget(x,y,w,h){init(); debprintf("Widget->%0x\n",this);};
    Widget(int x, int y, int z, int w, int h, int d, const char* l):EventTarget(x, y, z, w, h, d){init();}

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
		debprintf("%d %d hitMe test %0x  %d %d %d %d == %d\n", xi, yi, this, sx, sy, w(), h(), ret);
        return ret;
	}	
	virtual Widget* getObjOnFocus(int x, int y, int z){return this;}
	virtual int handleEvent(Event e){};
	virtual int handleKey(int key){};
    virtual int handleButton(ButtonEvent e){}
	virtual void redraw(){};
	//!
public:
	void getLBpos(int& leftX, int& bottomY, int& z){getSxyz(leftX, bottomY, z); bottomY -= h(); }
private:
	void init(){mParent = mNext = mPrevious = NULL;}
};
#endif
