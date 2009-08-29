#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include "font.h"
#include "string.h"
#include "ctrinput.h"
#include "display.h"
#define MAXCS 20
ctrInput::ctrInput()
{
}
ctrInput:: ~ctrInput()
{
	if(pvalue)free(pvalue);
}
void ctrInput::reset()
{
	maxc=64;
	curc= -1;
	curCursorPos=0;
	startDrawPos=0;
	curdx =0;
	focus = 0;
	pvalue = 0;
}

void ctrInput::initInput(int sdx, int sdy)
{
	maxc=1024;
	pvalue = (char *)malloc(maxc+1);
	memset(pvalue,0,maxc);
	curc= -1;
	curCursorPos=0;
	startDrawPos=0;
	curdx =0;
	pvalue[0]=0;
	focus = 0;
	debprintf("dx:%d\n",w());
    if(sdx==0) sdx = 10;
	w(sdx);
	h(sdy);
}

ctrInput::ctrInput(int sdx,int sdy):Widget(sdx,sdy)
{
		initInput(sdx, sdy);
}

ctrInput::ctrInput(int sx,int sy,int sz):Widget(sx,sy,sz)
{
	initInput(10,20 );
}

ctrInput::ctrInput(int sx,int sy,int sz,char *s):Widget(sx,sy,sz)
{
	initInput(10,20 );
	sprintf(pvalue,"%s",s);
}

ctrInput::ctrInput(int sx,int sy,int sz,int sdx,int sdy, char* l):Widget(sx,sy,sz,sdx,sdy,0)
{
	initInput(10,20 );
	sprintf(pvalue,"%s",l);
}

ctrInput::ctrInput(int sx,int sy,int w, int h, char *s):Widget(sx,sy,0,w,h,s)
{
	initInput(w,h );
}

ctrInput::ctrInput(int x,int y, int z, int w, int h, int d ,char *s):Widget(x,y,z,w,h,d,s)
{
	initInput(w,h );
}

void ctrInput::draw()
{
	int curRasterPos[4];
	GLfloat oldcolor[4];
	getLBpos(curRasterPos[0],curRasterPos[1],curRasterPos[2]);
	MoveTo2(curRasterPos[0],curRasterPos[1]-2);
	glGetFloatv(GL_CURRENT_COLOR,oldcolor);
	glColor4f(1,1,1,1);
	drawInput2d(curRasterPos[0],curRasterPos[1],0,w(),h());
	curRasterPos[0] += (w() + 1);
	putStrScr(pvalue + startDrawPos,curCursorPos-startDrawPos);
	drawCursor();
	putStrScr(pvalue + curCursorPos);
	MoveTo2(curRasterPos[0],curRasterPos[1]);
	glColor4fv(oldcolor);
}

void ctrInput::reDraw()
{
	//preRedraw();
	//GLfloat oldcolor[4];
	//glGetFloatv(GL_CURRENT_COLOR,oldcolor);
	glColor4f(1,1,1,1);
	int sx,sy,sz;
	//getSxyz(sx,sy,sz);
	getLBpos(sx,sy,sz);
#if 1 
	drawInput2d(sx,sy,0,w(),h());
	MoveTo2(sx,sy+3);
	putStrScr(pvalue + startDrawPos,curCursorPos-startDrawPos);
	drawCursor();
	putStrScr(pvalue + curCursorPos);
	copyToCurTexture( sx, sy, w(), h(), sx, sy);
	glutPostRedisplay();
#else
#define YoffS 1
#define XoffS 100
	drawInput2d(sx  ,sy+ YoffS,0,w(), h());
	MoveTo2(sx , sy+3);
	putStrScr(pvalue + startDrawPos,curCursorPos-startDrawPos);
	drawCursor();
	putStrScr(pvalue + curCursorPos);
	glReadBuffer(GL_BACK);
	glDrawBuffer(GL_FRONT);
	copyBackToFrt(sx, sy+YoffS, w(), h(), sx, sy+YoffS);
	glDrawBuffer(GL_BACK);
#endif
	//sufRedraw();
	//glColor4fv(oldcolor);

}
int  ctrInput::insertchar(char c)
{
	int i;
	if(curc >= maxc) return -1;
	curc++;
	curCursorPos ++;
	pvalue[curc+1]=0;
	for(i=curc;i > curCursorPos;i--)
		pvalue[i]=pvalue[i-1];
	pvalue[i]=c;

}
int  ctrInput::deletechar(char c)
{
}
int  ctrInput::backspacechar(char c)
{
	//if(c == 8)
#if 0
	{
		if(curc <0) return 0;
		curdx -= getCharWidth(pvalue[curc]);
		pvalue[curc]=0;
		curc--;
		curCursorPos--;
		if(curc < startDrawPos) {
			startDrawPos -= MAXCS;
			curdx = 0;
			if(startDrawPos <0) startDrawPos = 0;
			for(int i=startDrawPos;i<curCursorPos;i++)
				curdx +=getCharWidth(pvalue[i]);
		}
		return 0;
	}
#endif
#if 1
	if(curCursorPos ==0) return 0;
	char * ppcur = pvalue + curCursorPos -1;
	curdx -= getCharWidth(*ppcur);
	while(*(ppcur+1)){
		*ppcur = *(ppcur+1);
		printf("%c",*ppcur);
		ppcur++;
	}
	*ppcur =0;
	curCursorPos --;
	curc--;
	if(curCursorPos < startDrawPos) {
		startDrawPos -= MAXCS;
		curdx = 0;
		if(startDrawPos <0) startDrawPos = 0;
		for(int i=startDrawPos;i<curCursorPos;i++)
			curdx +=getCharWidth(pvalue[i]);
	}

#endif
}
int  ctrInput::appandchar(char c)
{
	if(curc >= maxc) return -1;
	curc++;
	curCursorPos++;
	pvalue[curc]=c;
	pvalue[curc+1]=0;
	curdx += getCharWidth(c);
	//if(curc - startDrawPos > MAXCS) {
	while(curdx >= w() -4){
		curdx -= getCharWidth(pvalue[startDrawPos]);
		startDrawPos++;
	}
	//printf("%d,%c\n",curdx,c);
	return 0;
}

INLINE void  ctrInput::drawCursor()
{
	//static int visible = 1;
	GLfloat oldcolor[4];
	GLint curpos[4];
#define CURSORW 8
#define CURSORH 20
	//if(!visible)
	{
		glGetFloatv(GL_CURRENT_COLOR,oldcolor);
		glColor4f(0,1.0,0.0,1);
        //glGetIntegerv(GL_CURRENT_RASTER_POSITION,curpos);
        getCurrentRasterPos(curpos);
        //curpos[2] = 0;
        glBegin(GL_QUADS);
        glVertex3f(curpos[0],curpos[1],curpos[2]);
        glVertex3f(curpos[0]+CURSORW,curpos[1],curpos[2]);
        glVertex3f(curpos[0]+CURSORW,curpos[1]+CURSORH,curpos[2]);
        glVertex3f(curpos[0],curpos[1]+CURSORH,curpos[2]);
        glEnd();
        glColor4fv(oldcolor);
    }
    //visible ++;
    //visible = visible & 1;
}
INLINE char * ctrInput::getValue()
{
    return pvalue;
}
/*
   INLINE void ctrInput::getFocus()
   {
   focus = 1;
   }
   INLINE void ctrInput::loseFocus()
   {
   focus = 0;
   }
   INLINE int ctrInput::onFocus()
   {
   return focus ;
   }
   */
/*
   INLINE int ctrInput::inArea(int ptx,int pty, int ptz )
   {
   if((ptx > x)&&(ptx <x +dx) && (pty >y)&&(pty <y +dy)) return 1;
   return 0;
   }
   */
INLINE void ctrInput::moveCursor(int drt)
{
    curCursorPos += drt;
    if(drt>0){
        if(curCursorPos > curc+1){ 
            curCursorPos = curc+1;
            return ;
        }
        curdx += getCharWidth(pvalue[curCursorPos-1]);
        //	if(curCursorPos - startDrawPos > MAXCS) startDrawPos++;	
        while(curdx >=  w() -4){
            curdx -= getCharWidth(pvalue[startDrawPos]);
            startDrawPos++;
        }
    }
    else{
        if(curCursorPos<0) {
            curCursorPos=0;
            return;
        }
        curdx -= getCharWidth(pvalue[curCursorPos]);
        if(curCursorPos < startDrawPos) {
            startDrawPos--;
            curdx = 0;
        }
    }
}
void ctrInput::keyfunc(char  key,int x,int y)
{
    if(key >31){
        if(curCursorPos == curc+1) 
            appandchar(key);
        else
            insertchar(key);
        printf("%s\n", pvalue);	
        return ;
    }
    switch ( key){
        case 8:
            backspacechar(key);
            break;
        case 27:
            loseFocus();
            break;
    }
}

INLINE void ctrInput::skeyfunc(int key,int x,int y)
{
    switch ( key){
        case GLUT_KEY_LEFT: 
            moveCursor(-1);
            break;
        case GLUT_KEY_RIGHT:
            moveCursor(1);
            break;
    }
}
