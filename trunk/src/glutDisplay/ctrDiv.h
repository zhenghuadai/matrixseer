#ifndef CTRDIV_HEADER
#define CTRDIV_HEADER
#ifndef INLINE
#define INLINE // inline 
#endif
#include <stdlib.h>
#include <stdio.h>
#include <FL/Fl.H>
#include "postionFlag.h"
//#define divBOX NO_BOX
//#define divBOX BORDER_FRAME
#define divBOX ENGRAVED_BOX
class ctrDiv : public Group {
	declarePFlag;
	int handle(int);
	int pos[4];
	public:
	int pushed ;
	ctrDiv(Box *b,int w,int h,const char *l)
		: Group(0,0,w,h,l) {box(b);pushed = 0;};
	ctrDiv(int w,int h,const char *l): Group(0,0,w,h,l){box(divBOX/*FL_SHADOW_FRAMEi*/);pushed = 0;}
	ctrDiv(int w,int h)
		: Group(0,0,w,h,(const char *)0) {pushed = 0;}
	ctrDiv(int x,int y,int w,int h,const char *l)
		: Group(x,y,w,h,l) {box(divBOX/*FL_SHADOW_FRAME*/);pushed = 0;setZeroPFlag();}
	ctrDiv(Box* b,int x,int y,int w,int h,const char *l)
		: Group(x,y,w,h,l) {}
	void setxyz(int x,int y, int z=0){position(x,y);}
	void setx(int x0){x(x0);}
	void sety(int y0){y(y0);}
	void setWH(int w0,int h0){w(w0);h(h0);}
	void size(int w0,int h0){w(w0);h(h0);}
	posFlagFunc;
	void setxyz(){
		extern void getCurrentRasterPos(int *pos);
		extern void forwardx(int x); 
		getCurrentRasterPos(pos);
#ifdef PDEB
		printf("[%d %d] ->", pos[0],pos[1]);
#endif
		position(pos[0],pos[1]);
	}
	void EndP(){
		extern void MoveTo2(int ,int);
		MoveTo2(x()+w(),y()+h());
		//   end();
#ifdef PDEB
		getCurrentRasterPos(pos);
		printf("[%d %d]\n", pos[0],pos[1]);
		//       Widget::position
#endif

	}
};

#endif
