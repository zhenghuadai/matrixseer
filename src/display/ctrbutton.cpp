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
}
ctrButton::ctrButton(int w,int h,char *pstr):Widget(w,h)
{
    pvalue=pstr;
}

ctrButton::ctrButton(int x, int y, int w, int h, char* pstr):Widget(x,y,w,h,pstr)
{
    pvalue=pstr;
}

ctrButton::ctrButton(int x, int y, int z, int w, int h, int d, char* pstr):Widget(x,y,z,w,h,d,pstr)
{
    pvalue=pstr;
}

ctrButton::~ctrButton()
{
    if(pvalue)free(pvalue);
}

void ctrButton::draw()
{
    int curRasterPos[4];
    int endPos[4];
	getLBpos(curRasterPos[0],curRasterPos[1],curRasterPos[2]);
	MoveTo2(curRasterPos[0],curRasterPos[1]+3);
    putStrScr(pvalue);
    getCurrentRasterPos(endPos);
    drawButton2dframe(curRasterPos[0],curRasterPos[1],curRasterPos[2], w(), h());
    endPos[0] += 1;
    MoveTo2(endPos[0],endPos[1]);
}
void ctrButton::redraw()
{
    preRedraw();
    GLfloat oldcolor[4];
    int sx, sy, sz;
    getLBpos(sx, sy, sz);
    glGetFloatv(GL_CURRENT_COLOR,oldcolor);
    glColor4f(.1,.1,.1,1.0);
    drawButton2d(sx,sy,0,w(),h());
    MoveTo2(sx,sy+3);
    glColor4f(1,.1,.1,1.0);
    putStrScr(pvalue ,strlen(pvalue));
    copyToCurTexture( sx, sy, w(), h(), sx, sy);

    glColor4fv(oldcolor);
    sufRedraw();
}
void ctrButton::keyfunc(int  key,int x,int y)
{
}
void ctrButton::OnClick()
{
    preRedraw();
    int sx, sy, sz;
    getLBpos(sx, sy, sz);
    drawInput2d(sx,sy,0,w(),h());
    MoveTo2(sx,sy+3);
    putStrScr(pvalue ,strlen(pvalue));
    copyToCurTexture( sx, sy, w(), h(), sx, sy);
    sufRedraw();

}
