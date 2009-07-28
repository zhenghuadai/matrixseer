#ifndef CTRINPUT_HEADER
#define CTRINPUT_HEADER
#ifndef INLINE
#define INLINE // inline 
#endif
#include "Widget.h"
class ctrInput: public Widget
{
public:
ctrInput();
ctrInput(int sdx,int sdy);
ctrInput(int sx,int sy, int sz);
ctrInput(int sx,int sy, int sz,int sdx,int sdy,char* l);
ctrInput(int sx,int sy,int sz,char * s);
ctrInput(int sx,int sy, int w,int h, char* l);
~ctrInput();
void reset();
//void setxyz(int sx,int sy,int sz,int sdx,int sdy);
int appandchar(char c);
virtual void draw();
void redraw();
//INLINE int inArea(int ptx,int pty, int ptz );
//INLINE void getFocus();
//INLINE void loseFocus();
//INLINE int  onFocus();
INLINE char *  getValue();
INLINE void drawCursor();
INLINE void moveCursor(int drt);
INLINE void skeyfunc(int key,int x,int y);
void keyfunc(char  key,int x,int y);
int  deletechar(char c);
int  insertchar(char c);
int  backspacechar(char c);
private:
//int x,y,z,dx,dy;
int maxc,curc;
int startDrawPos;
int curCursorPos;
int curdx;
//int focus;
char *pvalue;
};
#endif
