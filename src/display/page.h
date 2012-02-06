/*
 * =====================================================================================
 *
 *       Filename:  page.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/10/2011 03:20:05 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef  page_INC
#define  page_INC

#define GET_CUR_RASTER_POS()   getCurrentRasterPos((GLint*)&curRasterPos[curWin])

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


extern int Win_w;
extern int Win_h;
extern dmRect curRasterPos[10];
extern int    curWin;
void pushRect();
void popRect();
void getRect(int*);
void setRect(int,int,int,int);
int  getRectW();
int  getRectH();
int  getCurRectW();
void pushStartPos();
void popStartPos();
void setStartPos(int x,int y);
#endif   /* ----- #ifndef page_INC  ----- */
