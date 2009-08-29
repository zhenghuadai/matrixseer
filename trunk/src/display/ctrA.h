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
    virtual int handleButton(ButtonEvent e){if(state()!=CLICKED) setState(); }
    virtual void reDraw(){
        //Color oldColor= getColor();
        //preRedraw();
        int sx, sy, sz;
        getSxyz(sx,sy,sz);
        setColor(textColor());
        //glColor4f(1,.0,.0,1.0);
        putStrScr(sx, sy-getRowHeight(), sz, text());
        copyToCurTexture( sx, sy-h()-3, w(), h(), sx, sy-h()-3);
        //sufRedraw();
        //setColor(oldColor);
    }
    private:
    void setState(){ mState= CLICKED; textColor( Color((U8)255, 0,0,255));} 

};

#endif
