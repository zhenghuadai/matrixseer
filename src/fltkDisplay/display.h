#ifndef __DISPLAY__DMATRIX__
#define __DISPLAY__DMATRIX__
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
int putStrScr(const char *content,int x,int y,int w,int h,int offsetx);
//typedef struct _int2
//{
//    int w;
//    int h;
//}int2;
//
#endif
