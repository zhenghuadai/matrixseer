#ifndef CTRA_HEADER
#define CTRA_HEADER
#ifndef INLINE
#define INLINE // inline 
#endif
#include <stdlib.h>
#include <stdio.h>
#include "dmGlobal.h"
#include "Box.h"
#include "ctrText.h"
#include "Event.h"
class ctrA : public ctrText{
	private:
		int mState;
	public:
		int handle(int);
	public:
		ctrA(int w,int h,const char *l): ctrText(0,0,w,h,l)
		{box(BOX_BODER_SHAPE);mState = 0;}

		ctrA(int w,int h):ctrText (0,0,w,h,(const char *)0) 
		{box(BOX_BODER_SHAPE);mState = 0;}

		ctrA(int x,int y,int w,int h,const char *l): ctrText(x,y,w,h,l) 
		{box(BOX_BODER_SHAPE);mState = 0; }
		

		ctrA(Box* b,int x,int y,int w,int h,const char *l): ctrText(x,y,w,h,l) {}
		ctrA(int x,int y,int z, int w,int h,int d, const char *l): ctrText(x,y,z,w,h,d,l) {}
			
	public:
		int state(){return mState;}
	public:
	virtual int handleKey(int key){};
    virtual int handleButton(ButtonEvent e){ mState = CLICKED;}
	virtual void redraw(){ if (state() == CLICKED) glColor4f(1,0,.1,1.0); Output::draw(); glColor4f(.1,.1,.1,1.0);};
    

};

#endif
