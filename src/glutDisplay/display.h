#ifndef _MDISPLAY_HEADER__
#define _MDISPLAY_HEADER__
#define CUBICARCMODE 0 
#define CUBICMODE 1
#define ARCMODE   2
#define PLAINMODE 3
#include <GL/gl.h>
#include "dmtype.h"
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
#define curRasterPosX curRasterPos[curWin].x
#define curRasterPosY curRasterPos[curWin].y
#define curRasterPosZ curRasterPos[curWin].z

#define curOffsetX ((curRasterPosX) - (curRectX))

void drawToTexture2(GLuint tex,int Sx,int Sy,int Ex,int Ey,char *str);
void drawInput2d(double x,double y,double z,double dx,double dy);
void drawButton2d(double x,double y,double z,double dx,double dy);
void drawButton2dframe(double x,double y,double z,double dx,double dy);
int putStrScr(char *content);
int putStrScr(char *content,int len);
void drawCube6(double xleft,double xright,double ybottom,double ytop,double zfront,double zback);
int copyToCurTexture(int tx,int ty,int dsx,int dsy,int sw,int sh);
int copyBackToFrt(int stx,int sty,int dw,int dh,int destx,int dexty);
//int copyToCurTexture(int stx,int sty,int dx,int dy,int ssx,int ssy);
void preRedraw();
void sufRedraw();

#define FirstLineIndent 20
void pushRect();
void popRect();
void getRect(int*);
void setRect(int,int,int,int);
void getCurrentRasterPos(int *pos);
void forwardx(int x); 
void MoveTo2(int ,int);
void setStartPos(int x,int y);
void moveToNextLine();
void moveToNextLine(int dy);
void pushStartPos();
void popStartPos();
int  getRectW();
int  getRectH();
int getCurRectW();
int putStrScr(const char *content,int x,int y,int w,int h,int offsetx);
int2 getWH_Chars(int w,int h,int id);
#define GM_BLACK 0
#endif
