#ifndef MDISPLAY
#define MDISPLAY
#define CUBICARCMODE 0 
#define CUBICMODE 1
#define ARCMODE   2
#define PLAINMODE 3
#include <GL/gl.h>
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
#endif
