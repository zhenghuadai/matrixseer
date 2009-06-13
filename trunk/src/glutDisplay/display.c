#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <string.h>
#include <stdarg.h>
#include "font.h"
#include "bmp.h"
#include "html.h"
#include "display.h"
#include "app.h"
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

static int Tex_Win_h=0;
static int Tex_Win_w=0;


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

//
//void pushStartPos()
//{
//    topStartPos ++;
//    if(topStartPos >=20)return;
//    stackStartPos[topStartPos][0] = curStartPos[curWin][0];
//    stackStartPos[topStartPos][1] = curStartPos[curWin][1];
//    stackStartPos[topStartPos][2] = curStartPos[curWin][2];
//    stackStartPos[topStartPos][3] = curStartPos[curWin][3];
//}
//
//void popStartPos()
//{
//    if(topStartPos <0) return ;
//    curStartPos[curWin][0] = stackStartPos[topStartPos][0] ;
//    curStartPos[curWin][1] = stackStartPos[topStartPos][1] ;
//    curStartPos[curWin][2] = stackStartPos[topStartPos][2] ;
//    curStartPos[curWin][3] = stackStartPos[topStartPos][3] ;
//    topStartPos --;
//}
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

inline int2 getWH_Chars(int w,int h,int id)
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

void moveToNextLine(int dy)
{
    glGetIntegerv(GL_CURRENT_RASTER_POSITION,curRasterPos[curWin]);
    curRasterPosY-= dy;//rowHeight;
    curRasterPosX = curRectX;
    MoveTo2(curRasterPosX,curRasterPosY);
}

void moveToNextLine()
{
    glGetIntegerv(GL_CURRENT_RASTER_POSITION,curRasterPos[curWin]);
    curRasterPosY-= rowHeight;
    curRasterPosX = curRectX;
    MoveTo2(curRasterPosX,curRasterPosY);
}

void goDown(int y)
{
    glGetIntegerv(GL_CURRENT_RASTER_POSITION,curRasterPos[curWin]);
    curRasterPosY-= y;
    MoveTo2(curRasterPosX,curRasterPosY);
}

void forward0(int x,int y)
{
    curRasterPosX += x;
    curRasterPosY += y;
    MoveTo2(curRasterPosX,curRasterPosY);
}

void drawLine(double x,double y,double z,double x2,double y2,double z2)
{
    //glBegin(GL_LINE);
    glBegin(GL_LINE_STRIP);
    glVertex3d(x,y,z);
    glVertex3d(x2,y2,z2);
    glEnd();
}

void drawLine(double x,double y,double x2,double y2)
{
    glBegin(GL_LINE);
    glVertex3d(x,y,0.0);
    glVertex3d(x2,y2,.0);
    glEnd();
}

void drawFrame(double x,double y,double z,double dx,double dy)
{
    //   GLfloat oldcolor[4];
    //   glGetFloatv(GL_CURRENT_COLOR,oldcolor);
    glBegin(GL_LINE_STRIP);
    //    glColor4f(.8,.8,.8,1.0);
    glVertex3d(x,y,z);
    glVertex3d(x,y+dy,z);
    glVertex3d(x+dx,y+dy,z);
    glVertex3d(x+dx,y,z);
    glVertex3d(x,y,z);
    glEnd();
}
void drawButton2dframe(double x,double y,double z,double dx,double dy)
{
    GLfloat oldcolor[4];
    glGetFloatv(GL_CURRENT_COLOR,oldcolor);
    glBegin(GL_LINE_STRIP);
    glColor4f(.8,.8,.8,1.0);
    glVertex3d(x,y,z);
    glVertex3d(x,y+dy,z);
    glVertex3d(x+dx,y+dy,z);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glColor4f(.1,.1,.1,1.0);
    glVertex3d(x,y,z);
    glVertex3d(x+dx,y,z);
    glVertex3d(x+dx,y+dy,z);
    glEnd();
    glColor4fv(oldcolor);
}
void drawButton2d(double x,double y,double z,double dx,double dy)
{
    GLfloat oldcolor[4];
    glGetFloatv(GL_CURRENT_COLOR,oldcolor);
    glBegin(GL_QUADS);
    glColor4f(.9,.9,.9,1.0);
    glVertex3d(x,y,z);
    glVertex3d(x+dx,y,z);
    glVertex3d(x+dx,y+dy,z);
    glVertex3d(x,y+dy,z);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glColor4f(.8,.8,.8,1.0);
    glVertex3d(x,y,z);
    glVertex3d(x,y+dy,z);
    glVertex3d(x+dx,y+dy,z);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glColor4f(.1,.1,.1,1.0);
    glVertex3d(x,y,z);
    glVertex3d(x+dx,y,z);
    glVertex3d(x+dx,y+dy,z);
    glEnd();
    glColor4fv(oldcolor);
}
void drawInput2d(double x,double y,double z,double dx,double dy)
{
    GLfloat oldcolor[4];
    glGetFloatv(GL_CURRENT_COLOR,oldcolor);
    glBegin(GL_QUADS);
    glColor4f(.3,.3,.3,3.0);
    glVertex3d(x,y,z);
    glVertex3d(x+dx,y,z);
    glVertex3d(x+dx,y+dy,z);
    glVertex3d(x,y+dy,z);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glColor4f(.1,.1,.1,1.0);
    glVertex3d(x,y,z);
    glVertex3d(x,y+dy,z);
    glVertex3d(x+dx,y+dy,z);
    glEnd();
    glBegin(GL_LINE_STRIP);
    glColor4f(.8,.8,.8,1.0);
    glVertex3d(x,y,z);
    glVertex3d(x+dx,y,z);
    glVertex3d(x+dx,y+dy,z);
    glEnd();
    glColor4fv(oldcolor);
}
/*
   void drawInput(int dx,int dy)
   {
   glGetIntegerv(GL_CURRENT_RASTER_POSITION,curRasterPos[curWin]);
   drawInput2d(curRasterPosX,curRasterPosY+20,0,dx,dy);
   curRasterPosX += (dx + 1);
   MoveTo2(curRasterPosX,curRasterPosY);
   }
   */
int putStrScr(char * content,int width,int height)
    /*start from the left end
     *
     *
     *
     */
{
    int i=0,j=0;
    char *pstart=content;
    int tmpheight=height-20;
    cWH=getWH_Chars(width,height,3);

    if(!content)return 0;
    while(*pstart)
    {
        int npos=getEnterPos(pstart,cWH.w);
        glRasterPos2i(20, tmpheight-j*20);

        {
            char buf[20];
            sprintf(buf,"%d] ",tmpheight-j*20);
            drawChars(buf,strlen(buf));
        }

        drawChars(pstart,npos);
        pstart+=npos;
        if(*pstart=='\n')pstart++;
        j++;
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
    glGetIntegerv(GL_CURRENT_RASTER_POSITION,curRasterPos[curWin]);
    //	printf("%d,%d\n",curRasterPosX,curRasterPosY);
    tmpcWH=getWH_Chars(Tex_Win_w-curRasterPosX,Tex_Win_h,3);
    npos=getEnterPos(pstart,tmpcWH.w);
    npos = npos < len ? npos:len;
    drawChars(pstart,npos);
    pstart+=npos;
    if(pstart == pend)return 1;
    if((*pstart=='\n')&&(! (*(pstart+1)))){
        curRasterPosY-= rowHeight;
        MoveTo2(20,curRasterPosY);
        return 1; 
    }

    while((*pstart) &&(pstart < pend))
    {
        curRasterPosY-= rowHeight;
        MoveTo2(20,curRasterPosY);
        npos=getEnterPos(pstart,cWH.w);
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
    glGetIntegerv(GL_CURRENT_RASTER_POSITION,curRasterPos[curWin]);
    //	printf("%d,%d\n",curRasterPosX,curRasterPosY);
    tmpcWH=getWH_Chars(Tex_Win_w-curRasterPosX,Tex_Win_h,3);
    npos=getEnterPos(pstart,tmpcWH.w);
    drawChars(pstart,npos);
    pstart += npos;
    if((*pstart=='\n')&&(! (*(pstart+1)))){
        curRasterPosY-= rowHeight;
        MoveTo2(20,curRasterPosY);
        return 1; 
    }

    while(*pstart)
    {
        curRasterPosY-= rowHeight;
        MoveTo2(20,curRasterPosY);
        npos=getEnterPos(pstart,cWH.w);
        drawChars(pstart,npos);
        pstart+=npos;
        if(*pstart=='\n'){
            pstart++;
        }
    }
    return 1;
}
void drawButton(int dx,int dy,char * s)
{
    int startPos[4];
    int endPos[4];
    glGetIntegerv(GL_CURRENT_RASTER_POSITION,startPos);
    putStrScr(s);
    glGetIntegerv(GL_CURRENT_RASTER_POSITION,endPos);
    drawButton2d(startPos[0],startPos[1]-3,startPos[2],endPos[0] - startPos[0],22);
    endPos[0] += 1;
    MoveTo2(endPos[0],endPos[1]);
    //drawInput2d(startPos[0],startPos[1]+20,startPos[2],200,30);

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
    glDrawPixels(bmpinfo->bmiHeader.biWidth,bmpinfo->bmiHeader.biHeight,GL_RGB,GL_UNSIGNED_BYTE,data);
}

void renderBMP(int w,int h,const unsigned char * data)
{
    glDrawPixels(w,h,GL_RGB,GL_UNSIGNED_BYTE,data);
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
void drawToTexture(GLuint tex,int Sx,int Sy,int Ex,int Ey,char *str)
{
    putStrScr(str,Ex-Sx,Ey-Sy);
    glFlush ();
    glBindTexture(GL_TEXTURE_2D,tex);
    glCopyTexSubImage2D(GL_TEXTURE_2D,0,0,0,0,0, Ex-Sx,Ey-Sy);
    //glCopyTexSubImage2D(GL_TEXTURE_2D,0,0,0,0,0, 256, 256);
}
/*
 *for draw to texture  
 */
void setProjectionTex(int w,int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho (0.0, w, 0.0, h, -1.0, 1.0);
}
void setProjectionScr(int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1.0,1.0,-1.0,1.0,5.0,25.0);
}
void preDrawToTexture(GLuint tex,int Sx,int Sy,int Ex,int Ey)
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    Tex_Win_w=Ex-Sx;
    Tex_Win_h=Ey-Sy;
    setProjectionTex(Ex-Sx,Ey-Sy);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    glClearColor(1.0,1.0,1.0,1.0);
    glClear(GL_COLOR_BUFFER_BIT |  GL_DEPTH_BUFFER_BIT);
    cWH=getWH_Chars(Ex-Sx,Ey-Sy,3);
    //	printf("(%d,%d,%d,%d)\n",Sx,Sy,Ex,Ey);
    MoveTo2(Sx+20,Ey-20);
    setStartPos(Sx+20,Ey-20);
}
void afterDrawToTexture()
{
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glViewport(0, 0, (GLsizei) Win_w, (GLsizei) Win_h);
}
void drawScrToTexture(GLuint tex,int Sx,int Sy,int Ex,int Ey)
{
    glFlush ();
    glBindTexture(GL_TEXTURE_2D,tex);
    glCopyTexSubImage2D(GL_TEXTURE_2D,0,0,0,0,0, Ex-Sx,Ey-Sy);
}
void drawToTexture2(GLuint tex,int Sx,int Sy,int Ex,int Ey,char *str)
{
    preDrawToTexture(tex,Sx,Sy,Ex,Ey);
    //	putStrScr(str,Ex-Sx,Ey-Sy);
    //	putStrScr("i love you",Ex-Sx,Ey-Sy);
    /*
       putStrScr("1\n");
       putStrScr("2\n");
       putStrScr("3\n");
       drawInput(100,20,"i love you");
       putStrScr("4\n");
       putStrScr("\n");
       */
    //putStrScr(str);

    static	htmlparser hp;
    static int first =1;
    if(first){
        hp.parsehtml(str);
        first =0;
    }
    hp.renderhtml();
    //  drawInput2d(200,340,0,100,20);
    // drawButton2d(400,340,0,100,20);
    BITMAPINFO *bmpinfo;
    unsigned char * data;
    data=mLoadbmp("monkey.bmp",&bmpinfo);
    glRasterPos2f(200,20);
    renderBMP(bmpinfo,data);
    //    renderBMP(200,200,data);


    drawScrToTexture(tex,Sx,Sy,Ex,Ey);
    afterDrawToTexture();
}
void drawSquare5(double left,double right,double bottom,double top,double offset,int xyz)
{
    switch(xyz)
    {
        case 2://z=offset
            glBegin(GL_QUADS);
            //    glTexCoord2f (left,bottom);
            glVertex3f(left,bottom,offset);
            //    glTexCoord2f (right,bottom);
            glVertex3f(right,bottom,offset);
            //    glTexCoord2f (right,top);
            glVertex3f(right,top,offset);
            //    glTexCoord2f (left,top);
            glVertex3f(left,top,offset);
            glEnd();
            break;
        case 1://y=offset
            glBegin(GL_QUADS);
            //    glTexCoord2f (left,bottom);
            glVertex3f(left,offset,bottom);
            //    glTexCoord2f (right,bottom);
            glVertex3f(right,offset,bottom);
            //    glTexCoord2f (right,top);
            glVertex3f(right,offset,top);
            //    glTexCoord2f (left,top);
            glVertex3f(left,offset,top);
            glEnd();
            break;
        case 0://x=offset
            glBegin(GL_QUADS);
            //    glTexCoord2f (left,bottom);
            glVertex3f(offset,left,bottom);
            //    glTexCoord2f (right,bottom);
            glVertex3f(offset,right,bottom);
            //    glTexCoord2f (right,top);
            glVertex3f(offset,right,top);
            //    glTexCoord2f (left,top);
            glVertex3f(offset,left,top);
            glEnd();
            break;
    }
}
void drawCube6(double xleft,double xright,double ybottom,double ytop,double zfront,double zback)
{
    glNormal3f(0,-1,0);
    drawSquare5(xleft,xright,zback,zfront,ybottom,1);
    glNormal3f(0,1,0);
    drawSquare5(xleft,xright,zfront,zback,ytop,1);
    glNormal3f(-1,0,0);
    drawSquare5(ybottom,ytop,zfront,zback,xleft,0);
    glNormal3f(1,0,0);
    drawSquare5(ybottom,ytop,zback,zfront,xright,0);
    glNormal3f(0,0,1);
    drawSquare5(xleft,xright,ybottom,ytop,zfront,2);
    glNormal3f(0,0,-1);
    drawSquare5(xleft,xright,ytop,ybottom,zback,2);
}
void preRedraw()
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    setProjectionTex(Tex_Win_w,Tex_Win_h);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    /*
    */
}
void sufRedraw()
{
    afterDrawToTexture();
    /*
       glMatrixMode(GL_PROJECTION);
       glPopMatrix();
       glMatrixMode(GL_MODELVIEW);
       glPopMatrix();
       */  
}
GLint getTexref()
{

}
int copyBackToFrt(int stx,int sty,int dw,int dh,int destx,int desty)
{
    MoveTo2(destx,desty);
    glCopyPixels(stx,sty, dw,dh,GL_COLOR);
}
int copyToCurTexture(int tx,int ty,int dsx,int dsy,int sw,int sh)
{
    glBindTexture(GL_TEXTURE_2D,texName1);
    glCopyTexSubImage2D(GL_TEXTURE_2D,0,tx,ty,sw,sh, dsx,dsy);
}
/*
   void    printTex(GLubyte* tex1,int w,int h)
   {
   int i,j;
   for(i=0;i<w;i++)
   {
   for(j=0;j<h;j++)
   printf("(%d,%d,%d,%d)",tex1[i*w*4+j*4],tex1[i*w*4+j*4+1],tex1[i*w*4+j*4+2],tex1[i*w*4+j*4+3]);
   printf("\n");
   }
   }
   */
