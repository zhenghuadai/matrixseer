#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <string.h>
#include <stdarg.h>
#include "html.h"
//#include <FL/fl_draw.H>
//#include <fltk/run.h>
//#include <fltk/Window.h>
#include <fltk/Item.h>
#include <fltk/ValueSlider.h>
#include <fltk/draw.h>
#include <fltk/Choice.h>
//#include <fltk/Fl.h>
#include <fltk/compat/FL/fl_draw.H>
//#include <fltk/Fl_Choice.h>

#include "font.h"
#include "bmp.h"
#include "display.h"
#include "dmtype.h"
using namespace fltk;
class dmRect2D
{
    public:
    int x;
    int y;
    int w;
    int h;
    public:
    dmRect2D(){}
    private:
    void    setUndef(){x=y=w=h=-1;}

};
class dmRect 
{
    public:
    int x;
    int y;
    int z;
    int a;
    int w;
    int h;
    int d;
    public: 
    dmRect(){setUndef();}
    private:
    void    setUndef(){x=y=z=a=w=h=d=-1;}
};
/********************************
 *0:x;
 *1:y;
 *2:z;
 *3:
 *4:W
 *5:H
 *6:D
 * *******************************/

#if 0
#define curRectX curStartPos[curWin][0]
#define curRectY curStartPos[curWin][1]
#define curRectZ curStartPos[curWin][2]
#define curRectA curStartPos[curWin][3]
#define curRectW curStartPos[curWin][4]
#define curRectH curStartPos[curWin][5]
#define curRectD curStartPos[curWin][6]
#define  stackStartPosX stackStartPos[topStartPos][0] 
#define  stackStartPosY stackStartPos[topStartPos][1] 
#define  stackStartPosZ stackStartPos[topStartPos][2] 
#define  stackStartPosA stackStartPos[topStartPos][3]
#define  stackStartPosW stackStartPos[topStartPos][4]
#define  stackStartPosH stackStartPos[topStartPos][5]
#define  stackStartPosD stackStartPos[topStartPos][6]
#else
#define curRectX curStartPos[curWin].x
#define curRectY curStartPos[curWin].y
#define curRectZ curStartPos[curWin].z
#define curRectA curStartPos[curWin].a
#define curRectW curStartPos[curWin].w
#define curRectH curStartPos[curWin].h
#define curRectD curStartPos[curWin].d
#define  stackStartPosX stackStartPos[topStartPos].x  
#define  stackStartPosY stackStartPos[topStartPos].y
#define  stackStartPosZ stackStartPos[topStartPos].z
#define  stackStartPosA stackStartPos[topStartPos].a
#define  stackStartPosW stackStartPos[topStartPos].w
#define  stackStartPosH stackStartPos[topStartPos].h
#define  stackStartPosD stackStartPos[topStartPos].d
#define curRect curStartPos[curWin]
#define stackTopRect stackStartPos[topStartPos]  
#endif
#define curRasterPosX curRasterPos[curWin][0]
#define curRasterPosY curRasterPos[curWin][1]
#define curRasterPosZ curRasterPos[curWin][2]

#define curOffsetX ((curRasterPosX) - (curRectX))
extern int charWidth;
extern int mDC_w;
extern int mDC_h;
int rowHeight = 20;

static int2 fontSize={10,20};
static int curRasterPos[10][4]={0};
static dmRect curStartPos[10];
static int curWin=0;
static int2 cWH;
static dmRect stackStartPos[20];
static int topStartPos = -1;

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
    printf("CurRECT\nx:%d; y:%d z:%d, w:%d, h:%d, d:%d\n",
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

int getCurRectW()
{
    return curRectW; 
}
inline int2 getWH_Chars(int w,int h,int id)
{
    int2 tmp;
    tmp.w=w/charWidth - 2 ;
    tmp.h=h/5;
    return tmp;
}
void MoveTo2(int x,int y)
{
    curRasterPosX = x;
    curRasterPosY = y;
}
void MoveToxy(int x,int y)
{
    curRasterPosX = x;
    curRasterPosY = y;
}
void getCurrentRasterPos(int *pos)
{
    pos[0] = curRasterPosX; 
    pos[1] = curRasterPosY ;
    pos[2] = curRasterPosZ; 
}
#define LEFTMARGIN 4
void moveToNextLine(int dy)
{
    curRasterPosY += dy;//rowHeight;
    curRasterPosX = curRectX + LEFTMARGIN;
    MoveTo2(curRasterPosX,curRasterPosY);
}

void moveToNextLine()
{
    curRasterPosY += rowHeight;
    curRasterPosX = curRectX + LEFTMARGIN;
    MoveTo2(curRasterPosX,curRasterPosY);
}

void goDown(int y)
{
    curRasterPosY += y;
    MoveTo2(curRasterPosX,curRasterPosY);
}

void forwardx(int x)
{
#if DEB == 9
    printf("[%d",curRasterPosX);
#endif
    curRasterPosX += x;
#if DEB == 9
    printf(" + %d = %d]\n",x,curRasterPosX);
#endif
}

void forward0(int x,int y)
{
    curRasterPosX += x;
    curRasterPosY += y;
    MoveTo2(curRasterPosX,curRasterPosY);
}

void drawLine(int x,int y,int z,int x2,int y2,int z2)
{
    drawline(x,y,x2,y2); 
}

void drawLine(int x,int y,int x2,int y2)
{
    drawline(x,y,x2,y2); 
}

void drawLine(double x,double y,double z,double x2,double y2,double z2)
{
    //    fl_line(x,y,x2,y2); 
}

void drawLine(double x,double y,double x2,double y2)
{
    //  fl_line(x,y,x2,y2); 
}

//void drawFrame(double x,double y,double z,double dx,double dy)
void drawFrame(int x,int y,int z,int dx,int dy)
{
    fl_begin_line();
    fl_vertex(x,y);
    fl_vertex(x,y+dy);
    fl_vertex(x+dx,y+dy);
    fl_vertex(x+dx,y);
    fl_vertex(x,y);
    fl_end_line();
}
int putStrScr(char * content,int width,int height)
    /*start from the left end
     *
     *
     *
     */
{
    int i=0,j=0;
    char *pstart=content;
    int npos;

    int2 tmpcWH;	
    if((!content)||(! *content))return 0;
    //	printf("%d,%d\n",curRasterPosX,curRasterPosY);
    //tmpcWH=getWH_Chars(curRectW-curRasterPosX,mDC_h,3);
    //npos=getEnterPos(pstart,tmpcWH.w);
    npos = getLenInW(pstart,width-(curRasterPosX-curRectX));
    drawChars(pstart,npos);
    curRasterPosX += getCharsWidth(pstart,npos);
    if((*pstart=='\n')&&(! (*(pstart+1)))){
        curRasterPosY-= rowHeight;
        MoveTo2(curRectX ,curRasterPosY);
        return 1; 
    }

    while(*pstart)
    {
        curRasterPosY-= rowHeight;
        MoveTo2(curRectX,curRasterPosY);
        //    npos=getEnterPos(pstart,cWH.w);
        //    npos = getLenInW(pstart,curRectW-curRasterPosX);
        npos = getLenInW(pstart,width-(curRasterPosX-curRectX));
        drawChars(pstart,npos);
        pstart+=npos;
        if(*pstart=='\n'){
            pstart++;
        }
    }
    return 1;
}
int putStrScr(char *content,int len)
    /*start from current pos
     *
     *
     *
     */
{
    int i=0,j=0;
    char *pstart=content;
    char *pend  = pstart+len;
    int npos;
    int2 tmpcWH;	
    if((!content)||(! *content))return 0;
    //	printf("%d,%d\n",curRasterPosX,curRasterPosY);
    // tmpcWH=getWH_Chars(curRectW-curRasterPosX,mDC_h,3);
    // npos=getEnterPos(pstart,tmpcWH.w);
    npos = getLenInW(pstart,curRectW-curRasterPosX);
    npos = npos < len ? npos:len;
    drawChars(pstart,npos);
    pstart+=npos;
    if(pstart == pend)return 1;
    if((*pstart=='\n')&&(! (*(pstart+1)))){
        curRasterPosY-= rowHeight;
        //MoveTo2(20,curRasterPosY);
        MoveTo2(curRectX+20,curRasterPosY);
        return 1; 
    }

    while((*pstart) &&(pstart < pend))
    {
        curRasterPosY-= rowHeight;
        //MoveTo2(20,curRasterPosY);
        MoveTo2(curRectX+20,curRasterPosY);
        //    npos=getEnterPos(pstart,cWH.w);
        npos = getLenInW(pstart,curRectW-curRasterPosX);
        npos = npos < len ? npos:len;
        drawChars(pstart,npos);
        pstart+=npos;
        if(pstart == pend) return 1;
        if(*pstart=='\n'){
            pstart++;
        }
    }
    return 1;
}
int putStrScr(char *content)
    /*start from current pos
     *
     *
     *
     */
{
    int i=0,j=0;
    char *pstart=content;
    int npos;

    int2 tmpcWH;	
    if((!content)||(! *content))return 0;
    //npos = getLenInW(pstart,curRectW-curRasterPosX);
    npos = getLenInW(pstart,curRectW-curOffsetX);
    drawChars(pstart,npos);
    // curRasterPosX += getCharsWidth(pstart,npos);
    pstart+=npos;
    if(*pstart=='\n'){
        pstart++;
    }
    while(*pstart)
    {
        curRasterPosY += rowHeight;
        if(*(pstart-1) == '\n')
            MoveTo2(curRectX+FirstLineIndent,curRasterPosY);
        else
            MoveTo2(curRectX+LEFTMARGIN,curRasterPosY);
        //    npos=getEnterPos(pstart,cWH.w);
        npos = getLenInW(pstart,curRectW-curOffsetX);
        //    npos = getLenInW(pstart,curRectW-curRasterPosX);
        drawChars(pstart,npos);
        pstart += npos;
        if(*pstart=='\n'){
            pstart++;
        }
    }
    return 1;
}

int putStrScr(const char *content,int x,int y,int w,int h,int offsetx)
    /*start from current pos
     *
     *
     *
     */
{
    int i=0,j=0;
    char *pstart=(char *)content;
    int npos;
    int curY = y;
    int curX = x+offsetx;
    int2 tmpcWH;	
    if((!content)||(! *content))return 0;
    //npos = getLenInW(pstart,curRectW-curRasterPosX);
    npos = getLenInW(pstart,w - offsetx);
    dm_drawString(pstart,npos,curX,curY);
    // curRasterPosX += getCharsWidth(pstart,npos);
    pstart+=npos;
    if(*pstart=='\n'){
        pstart++;
    }
    while(*pstart)
    {
        curY += rowHeight;
        if(*(pstart-1) == '\n')
            curX = x+ FirstLineIndent; 
        else
            curX = x+ LEFTMARGIN;
        //    npos=getEnterPos(pstart,cWH.w);
        npos = getLenInW(pstart,w-(curX-x));
        //    npos = getLenInW(pstart,curRectW-curRasterPosX);
        dm_drawString(pstart,npos,curX,curY);
        pstart += npos;
        if(*pstart=='\n'){
            pstart++;
        }
    }
    return 1;
}

int Disprintf(char * format,...)
{
    va_list ap;
    char *buf;
    buf=(char *) malloc(4096);
    va_start(ap,format);
    vsprintf(buf,format, ap);
    va_end(ap);
    putStrScr(buf);
    free(buf);
    return 1;
}

void renderBMP(BITMAPINFO * bmpinfo,const unsigned char * data)
{
    int x = curRasterPosX ;
    int y = curRasterPosY ;
    //  fl_draw_image(NULL,data,x,y,bmpinfo->bmiHeader.biWidth,bmpinfo->bmiHeader.biHeight);
}

void renderBMP(int w,int h,const unsigned char * data)
{
    int x = curRasterPosX ;
    int y = curRasterPosY ;
    //    fl_draw_image(NULL,data,x,y,w,h);
}

void drawPic(char *picFn,int w,int h)
{
    BITMAPINFO *bmpinfo;
    unsigned char * data;
    data=mLoadbmp("monkey.bmp",&bmpinfo);
    //renderBMP(bmpinfo,data);
    if(data)
        renderBMP(w,h,data);
}
int  getColor()
{
    return fl_color();
}
int setColor(int color)
{
    int old = fl_color();
    fl_color(color);
    return old;
    //    fl_color(FL_BLUE);
}
