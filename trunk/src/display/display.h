#ifndef _MDISPLAY_HEADER__
#define _MDISPLAY_HEADER__
#define CUBICARCMODE 0 
#define CUBICMODE 1
#define ARCMODE   2
#define PLAINMODE 3
#include "dmtype.h"
#include "font.h"
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
void drawInput2d(double x,double bottomY,double z,double dx,double dy);
void drawButton2d(double x,double bottomY,double z,double dx,double dy);
void drawButton2dframe(double x,double bottomYy,double z,double dx,double dy);
int putStrScr(char *content);
int putStrScr(char *content,int len);
int putStrScr(int x, int y, int z, char *content);
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
int getWinw();
int getWinh();
int getRowHeight();
Color getColor();
void setColor(Color);
void dmPostRedisplay();
#define GM_BLACK 0
#endif
