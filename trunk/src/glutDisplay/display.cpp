#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <string.h>
#include <stdarg.h>
#include "font.h"
#include "bmp.h"
#include "html.h"
#include "display.h"
/********************************
 *0:x;
 *1:y;
 *2:z;
 *3:
 *4:W
 *5:H
 *6:D
 * *******************************/
extern int charWidth;
int mDC_w;
int mDC_h;
int rowHeight = 20;

int2 fontSize={10,20};
dmRect curRasterPos[10];
dmRect curStartPos[10];
dmRect stackStartPos[20];
int curWin=0;
int2 cWH;
int topStartPos = -1;

int Tex_Win_h=0;
int Tex_Win_w=0;

extern int Win_w;
extern int Win_h;

void resetDisplay()
{
    curRasterPosX = 0;
    curRasterPosY = 10;
}

void initDisplay()
{
    curRectX = 0;
    curRectY = 10;
    curRectZ = 0;
    curRectA = 0;
    mDC_w = Win_w;
    mDC_h = Win_h;
    curRectW = mDC_w;
    curRectH = mDC_h;
}

void pushStartPos()
{
    topStartPos ++;
    if(topStartPos >=20)return;
    stackStartPosX = curRectX;
    stackStartPosY = curRectY;
    stackStartPosZ = curRectZ;
    stackStartPosA = curRectA;
}

void popStartPos()
{
    if(topStartPos <0) return ;
    curRectX = stackStartPosX ;
    curRectY = stackStartPosY ;
    curRectZ = stackStartPosZ ;
    curRectA = stackStartPosA ;
    topStartPos --;
}

void printRect()
{
    debprintf("CurRECT\nx:%d; y:%d z:%d, w:%d, h:%d, d:%d\n",
            curRectX,
            curRectY,
            curRectZ,
            curRectW,
            curRectH,
            curRectD);
}

void pushRect()
{
    topStartPos ++;
    if(topStartPos >=20)return;
    stackTopRect = curRect; 
}

void popRect()
{
    if(topStartPos <0) return ;
    curRect = stackTopRect;
    topStartPos --;
}

void getRect(int *rect)
{
    //rect[0] = curRectX;
    //rect[1] = curRectY;
    //rect[2] = curRectZ;
    //rect[3] = curRectA;
    //rect[4] = curRectW;
    //rect[5] = curRectH;
    *(dmRect*)rect = curRect;
}
int getRectW()
{
    return curRectW;
}
int getRectH()
{
    return curRectH;
}
void setRect(int x,int y,int w,int h)
{
    curRectX = x;
    curRectY = y;
    curRectW = w;
    curRectH = h;

}

void setStartPos(int x,int y,int z)
{
    curRectX=x;
    curRectY=y;
    curRectZ=z;
}

void setStartPos(int x,int y)
{
    curRectX=x;
    curRectY=y;
}

int2 getWH_Chars(int w,int h,int id)
{
    int2 tmp;
    tmp.w=w/charWidth - 2 ;
    tmp.h=h/5;
    return tmp;
}

void MoveToxy(int x,int y)
{
    glRasterPos2i(x,y);
}

void getCurrentRasterPos(int *pos)
{
    glGetIntegerv(GL_CURRENT_RASTER_POSITION,pos);
}

int getCurRectW()
{
    return curRectW; 
}

void moveToNextLine(int dy)
{
    glGetIntegerv(GL_CURRENT_RASTER_POSITION,(GLint*)&curRasterPos[curWin]);
    curRasterPosY-= dy;//rowHeight;
    curRasterPosX = curRectX;
    MoveTo2(curRasterPosX,curRasterPosY);
}

void moveToNextLine()
{
    glGetIntegerv(GL_CURRENT_RASTER_POSITION,(GLint*)&curRasterPos[curWin]);
    curRasterPosY-= rowHeight;
    curRasterPosX = curRectX;
    MoveTo2(curRasterPosX,curRasterPosY);
}

void goDown(int y)
{
    glGetIntegerv(GL_CURRENT_RASTER_POSITION,(GLint*)&curRasterPos[curWin]);
    curRasterPosY-= y;
    MoveTo2(curRasterPosX,curRasterPosY);
}

void forward0(int x,int y)
{
    curRasterPosX += x;
    curRasterPosY += y;
    MoveTo2(curRasterPosX,curRasterPosY);
}
int getRowHeight()
{
    return rowHeight;
}
