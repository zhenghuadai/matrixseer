#include "ctrbutton.h"
#include "display.h"
#include "font.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
ctrButton::ctrButton():Widget()
{
}
ctrButton::ctrButton(int x,int y,int z):Widget(x,y,z)
{
}
ctrButton::ctrButton(int w,int h):Widget(w,h)
{
    curc=0;
}
ctrButton::ctrButton(int w,int h,char *pstr):Widget(w,h)
{
    pvalue=pstr;
    curc = strlen(pvalue);
}
ctrButton::~ctrButton()
{
    if(pvalue)free(pvalue);
}
void ctrButton::draw()
{
    int curRasterPos[4];
    int endPos[4];
	getSxyz(curRasterPos[0],curRasterPos[1],curRasterPos[2]);
	MoveTo2(curRasterPos[0],curRasterPos[1]-2);
    putStrScr(pvalue);
    getCurrentRasterPos(endPos);
    _w = endPos[0] - curRasterPos[0];
    _h = 22;
    drawButton2dframe(curRasterPos[0],curRasterPos[1]-3,curRasterPos[2],_w,22);
    endPos[0] += 1;
    MoveTo2(endPos[0],endPos[1]);
}
void ctrButton::redraw()
{
    preRedraw();
    GLfloat oldcolor[4];
    glGetFloatv(GL_CURRENT_COLOR,oldcolor);
    glColor4f(.1,.1,.1,1.0);
    drawButton2d(_x,_y,0,_w,_h);
    MoveTo2(_x,_y+3);
    glColor4f(1,.1,.1,1.0);
    putStrScr(pvalue ,curc);
    copyToCurTexture( _x, _y, _w, _h, _x, _y);

    glColor4fv(oldcolor);
    sufRedraw();
}
void ctrButton::keyfunc(int  key,int x,int y)
{
}
void ctrButton::OnClick()
{
    preRedraw();
    drawInput2d(_x,_y,0,_w,_h);
    MoveTo2(_x,_y+3);
    putStrScr(pvalue ,curc);
    copyToCurTexture( _x, _y, _w, _h, _x, _y);
    sufRedraw();

}