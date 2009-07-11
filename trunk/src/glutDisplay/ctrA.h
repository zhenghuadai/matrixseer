#ifndef CTRA_HEADER
#define CTRA_HEADER
#ifndef INLINE
#define INLINE // inline 
#endif
#include <stdlib.h>
#include <stdio.h>
#include "dmGlobal.h"
#include "Box.h"
class ctrA : public Widget {
	int handle(int);
	public:
	int pushed ;
	void draw(){};
	ctrA(Box* b,int w,int h,const char *l)
		: Widget(0,0,w,h,l) 
	{box(b);pushed = 0;};

	ctrA(int w,int h,const char *l)
		: Widget(0,0,w,h,l)
	{box(BOX_BODER_SHAPE);pushed = 0;}

	ctrA(int w,int h)
		: Widget(0,0,w,h,(const char *)0) 
	{box(BOX_BODER_SHAPE);pushed = 0;}

	ctrA(int x,int y,int w,int h,const char *l)
		: Widget(x,y,w,h,l) 
	{box(BOX_BODER_SHAPE);pushed = 0; 
		//labelsize(16);
	}

	ctrA(Box* b,int x,int y,int w,int h,const char *l)
		: Widget(x,y,w,h,l) 
	{}
	void setxyz(int x,int y, int z=0){position(x,y);}
	void setxyz(){
		int pos[4];
		extern void getCurrentRasterPos(int *pos);
		//extern void forwardx(int x); 
		getCurrentRasterPos(pos);
		position(pos[0],pos[1]);
		//forwardx(w());
	}
};

#endif
